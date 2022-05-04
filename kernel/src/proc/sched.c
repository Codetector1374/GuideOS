#include "proc.h"
#include "defs.h"
#include "mem/page_alloc.h"
#include "mem/vm.h"
#include "param.h"
#include "spinlock.h"
#include "arch/x86/interrupt.h"
#include "arch/x86/instructions.h"


struct scheduler_queue_node {
  struct proc* p;
  struct scheduler_queue_node *next;
};

struct scheduler_queue {
  size_t size;
  struct scheduler_queue_node* head;
  struct scheduler_queue_node* tail;
};

struct scheduler {
  struct scheduler_queue ready_queue;
  spinlock_t lock;
} sched;

void sched_init(void) {
  // Initialize Scheduler
  init_lock(&sched.lock, "sched");
  sched.ready_queue.size = 0;
  sched.ready_queue.head = NULL;
  sched.ready_queue.tail = NULL;
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

static void s_sched_add(struct proc* proc) {
  struct scheduler_queue_node* node = kmalloc(sizeof(struct scheduler_queue_node));
  node->p = proc;
  node->next = NULL;
  if (sched.ready_queue.size == 0) {
    sched.ready_queue.head = node;
  } else {
    sched.ready_queue.tail->next = node;
  }
  sched.ready_queue.tail = node;
  sched.ready_queue.size++;
}

void sched_add(struct proc* proc) {
  if (proc->state != RUNNABLE) {
    panic("sched_add not ready");
  }
  acquire(&sched.lock);
  s_sched_add(proc);
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
      sched.ready_queue.size--;
      struct scheduler_queue_node* head = sched.ready_queue.head;
      p = head->p;
      sched.ready_queue.head = head->next;
      if (sched.ready_queue.size == 0) {
        sched.ready_queue.tail = NULL;
      }
      kfree(head);
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
        (c->proc->state == RUNNABLE) && 
        (c->proc != &idle_proc[cpu_id()])) {
      // Add current proc to ready queue if it is marked runnable
      s_sched_add(c->proc);
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
    
  }
  sched_switch_out();
  release(&sched.lock);
}
