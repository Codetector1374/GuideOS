#include "proc.h"
#include "defs.h"
#include "mem/page_alloc.h"
#include "mem/vm.h"
#include "param.h"
#include "spinlock.h"
#include "arch/x86/interrupt.h"

struct ptable {
  spinlock_t lock;
  int next_pid;
  struct proc proc[NPROC];
  struct proc idle_proc[NCPU];
} ptable;

void proc_init(void) {
  init_lock(&ptable.lock, "ptable");
  memset(ptable.proc, 0, sizeof(ptable.proc));
  memset(ptable.idle_proc, 0, sizeof(ptable.idle_proc));
  ptable.next_pid = 1;
}

__attribute__((noreturn)) static void idle_function(void) {
  kprintf("Starting idle process...\n");
  for (;;) {
    wait_for_interrupt();
  }
}

static void sched_setup_idle_proc(struct proc* idleProc) {
  void* idleKStack = pgalloc_allocate(0);
  void* idleKStackTop = (void*)(((size_t)idleKStack) + PG_SIZE);
  memcpy(idleProc->name, "idle", 5);
  idleProc->killed = 0;
  idleProc->kstack = idleKStackTop;
  idleProc->pgtable = vmm_get_kernel_pgtable();
  idleProc->pid = 0;
  idleProc->state = RUNNABLE;
  idleProc->type = KERNEL;

  trapframe_t* tf = &(idleProc->tf);
  memset(tf, 0, sizeof(trapframe_t));
  tf->rip = (size_t) idle_function;
  tf->rsp = (size_t) idleProc->kstack;
  tf->cs = GDT_KERNEL_CODE * 8;
  tf->ss = GDT_KERNEL_DATA * 8;
}

void sched_start(void) {
  int cpu_idx = cpu_id();
  struct proc* this_idle_proc = &ptable.idle_proc[cpu_idx];
  sched_setup_idle_proc(this_idle_proc);
  cur_cpu()->proc = this_idle_proc;
  asm volatile("xchg %bx, %bx");
  return_to_trapframe(&this_idle_proc->tf);
}
