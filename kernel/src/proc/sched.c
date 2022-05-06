#include "proc.h"
#include "defs.h"
#include "mem/page_alloc.h"
#include "mem/vm.h"
#include "param.h"
#include "spinlock.h"
#include "arch/x86/interrupt.h"
#include "arch/x86/instructions.h"
#include "util/queue.h"


struct scheduler {
  dequeue_t ready_queue;
  dequeue_t waiting_queue;

  spinlock_t lock;
} sched;

void sched_init(void) {
  // Initialize Scheduler
  init_lock(&sched.lock, "sched");
  acquire(&sched.lock);
  dequeue_construct(&sched.ready_queue);
  dequeue_construct(&sched.waiting_queue);
  release(&sched.lock);
}

__attribute__((noreturn)) static void idle_function(void) {
  for (;;) {
    wait_for_interrupt();
  }
}


void proc_kentry(void) {
  struct proc* p = curproc();
  if (p == NULL) {
    panic("no curproc?");
  }
  release(&sched.lock);
  // return to "caller"
}

void sched_create_kproc(struct proc* proc, void (*func)(void)) {
  memcpy(proc->name, "idle", 5);
  proc->killed = 0;
  proc->pgtable = vmm_get_kernel_pgtable();
  proc->state = RUNNABLE;
  proc->type = KERNEL;
  proc->ctx->rip = (size_t)(proc_kentry);

  trapframe_t* tf = proc->tf;
  memset(tf, 0, sizeof(trapframe_t));
  tf->rip = (size_t) func;
  tf->rsp = (size_t) proc->kstack_top;
  tf->cs = GDT_KERNEL_CODE * 8;
  tf->ss = GDT_KERNEL_DATA * 8;
  tf->flags = RFLAGS_IF;
}

static void sched_setup_idle_proc(struct proc* idleProc) {
  proc_kern_init(idleProc);
  idleProc->pid = 0;
  sched_create_kproc(idleProc, idle_function);
}

void sched_add(struct proc* proc) {
  if (proc->state != RUNNABLE) {
    panic("sched_add not ready");
  }
  acquire(&sched.lock);
  dequeue_push_back(&sched.ready_queue, proc);
  release(&sched.lock);
}

void sched_start(void) {
  struct proc *p;
  struct cpu* c = cur_cpu();
  c->proc = NULL;

  // Setup Idle proc
  int cpu_idx = cpu_id();
  struct proc* this_idle_proc = &idle_proc[cpu_idx];
  this_idle_proc->state = RUNNING;
  sched_setup_idle_proc(this_idle_proc);
  
  enable_interrupt();

  acquire(&sched.lock);
  for(;;) {

    p = NULL;

    if (sched.ready_queue.size > 0) {
      p = dequeue_pop_front(&sched.ready_queue);
    }

    if (p == NULL) {
      p = &idle_proc[cpu_id()];
    }

    // Picked the new process to run
    c->proc = p;
    p->state = RUNNING;
    // TODO: swap pgtable
    sched_ctxsw(&(c->scheduler_ctx), p->ctx);

    // Here when the proc done running
    vmm_load_ktable();
    if ((c->proc != NULL) && 
        (c->proc != &idle_proc[cpu_id()])) {
        // Add current proc to ready queue if it is marked runnable else goes in waiting queue
      if (c->proc->state == RUNNABLE) {
        dequeue_push_back(&sched.ready_queue, c->proc);
      } else {
        dequeue_push_back(&sched.waiting_queue, c->proc);
      }
    }
    c->proc = NULL;
  }
  release(&sched.lock);
}

void sched_switch_out(void) {
  int intena;
  struct proc* p = curproc();

  if (!holding(&sched.lock)) {
    panic("sched_swtich_out lock");
  }
  if (cur_cpu()->int_disable_layer != 1) {
    panic("switch_out lock count");
  }
  if (p->state == RUNNING) {
    panic("switching out running");
  }
  intena = cur_cpu()->int_enabled;
  sched_ctxsw(&p->ctx, cur_cpu()->scheduler_ctx);
  cur_cpu()->int_enabled = intena;
}

void sched_yield(void) {
  acquire(&sched.lock);
  if (curproc()->state == RUNNING) {
    curproc()->state = RUNNABLE;
  }
  sched_switch_out();
  release(&sched.lock);
}

void sched_sleep(void* chan, spinlock_t *lk) {
  struct proc* p = curproc();
  if (p == NULL) {
    panic("sched_sleep");
  }
  if (lk == NULL) {
    panic("sleep w/o lock");
  }

  if (lk != &sched.lock) {
    acquire(&sched.lock);
    release(lk);
  }

  p->chan = chan;
  p->state = SLEEPING;

  sched_switch_out();

  p->chan = NULL;

  if (lk != &sched.lock) {
    acquire(lk);
    release(&sched.lock);
  }
}

void sched_wakeup(void* chan) {
  acquire(&sched.lock);
  dequeue_iterator_t iter;
  dequeue_iterator(&sched.waiting_queue, &iter);
  while(dequeue_iterator_has_next(&iter)) {
    struct proc* p = dequeue_iterator_next(&iter);
    if (p->chan == chan) {
      dequeue_iterator_delete(&iter);
      dequeue_push_back(&sched.ready_queue, p);
    }
  }
  release(&sched.lock);
}
