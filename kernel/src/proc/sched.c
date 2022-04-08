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

void sched_create_kproc(struct proc* proc, void (*func)(void)) {
  void* kStack = pgalloc_allocate(0);
  void* kStackTop = (void*)(((size_t)kStack) + PG_SIZE);
  memcpy(proc->name, "idle", 5);
  proc->killed = 0;
  proc->kstack = kStackTop;
  proc->pgtable = vmm_get_kernel_pgtable();
  proc->state = RUNNABLE;
  proc->type = KERNEL;

  trapframe_t* tf = &(proc->tf);
  memset(tf, 0, sizeof(trapframe_t));
  tf->rip = (size_t) func;
  tf->rsp = (size_t) proc->kstack;
  tf->cs = GDT_KERNEL_CODE * 8;
  tf->ss = GDT_KERNEL_DATA * 8;
  tf->flags = RFLAGS_IF;
}

static void sched_setup_idle_proc(struct proc* idleProc) {
  idleProc->pid = 0;
  sched_create_kproc(idleProc, idle_function);
}

void sched_start(void) {
  int cpu_idx = cpu_id();
  struct proc* this_idle_proc = &idle_proc[cpu_idx];
  this_idle_proc->state = RUNNING;
  sched_setup_idle_proc(this_idle_proc);
  cur_cpu()->proc = this_idle_proc;
  kprintf("cpu %d starting...\n", cpu_idx);
  return_to_trapframe(&this_idle_proc->tf);
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

void sched_switch(trapframe_t *tf) {
  push_int_disable();
  struct cpu* currentCpu = cur_cpu();
  pop_int_disable();
  struct proc* currentProc = curproc();
  struct proc* newProc = NULL;
  acquire(&sched.lock);

  // Save state and schedule out
  if (currentProc->state == RUNNING) {
    panic("switching out running?");
  }
  if (currentProc->state == RUNNABLE && currentProc != &idle_proc[cpu_id()]) {
    s_sched_add(currentProc);
  }

  if (sched.ready_queue.size > 0) {
    sched.ready_queue.size--;
    struct scheduler_queue_node* head = sched.ready_queue.head;
    newProc = head->p;
    sched.ready_queue.head = head->next;
    if (sched.ready_queue.size == 0) {
      sched.ready_queue.tail = NULL;
    }
    kfree(head);
  }

  if (newProc == NULL) {
    newProc = &idle_proc[cpu_id()];
  }

  newProc->state = RUNNING;
  if (newProc != currentProc) {
    currentProc->tf = *tf;
    *tf = newProc->tf;
    currentCpu->proc = newProc;
  }
  release(&sched.lock);
}

