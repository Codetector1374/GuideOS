#include "proc.h"
#include "defs.h"
#include "mem/page_alloc.h"
#include "mem/vm.h"
#include "param.h"
#include "spinlock.h"
#include "arch/x86/interrupt.h"
#include "arch/x86/instructions.h"

struct ptable {
  spinlock_t lock;
  int next_pid;
  struct proc proc[NPROC];
} ptable;

struct proc idle_proc[NCPU];

void proc_init(void) {
  // Initialize Process Table
  init_lock(&ptable.lock, "ptable");
  memset(ptable.proc, 0, sizeof(ptable.proc));
  memset(idle_proc, 0, sizeof(idle_proc));
  ptable.next_pid = 1;

  sched_init();
}

struct proc* proc_alloc(void) {
  struct proc* selected = NULL;
  acquire(&ptable.lock);
  for(size_t i = 0; i < NPROC; i++) {
    if (ptable.proc[i].state == UNUSED) {
      selected = &ptable.proc[i];
      memset(selected, 0, sizeof(struct proc));
      selected->state = CREATION;
      selected->pid = (ptable.next_pid++);
      break;
    }
  }
  release(&ptable.lock);
  return selected;
}

void wait(void* chan) {
  struct proc* currentProc = curproc();
  acquire(&ptable.lock);
  currentProc->chan = chan;
  currentProc->state = SLEEPING;
  release(&ptable.lock);
  asm volatile("int $0x80");
}
