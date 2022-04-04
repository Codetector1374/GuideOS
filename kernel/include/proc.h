#pragma once

#include "types.h"
#include "mem/vm.h"
#include "arch/x86/vmm.h"
#include "arch/x86/interrupt.h"

enum proc_state { UNUSED, CREATION, SLEEPING, RUNNABLE, RUNNING, ZOMBIE};
enum proc_type { KERNEL, USER };

struct proc {
  pml4e_t *pgtable;  // Root Page Table
  trapframe_t tf;
  void *kstack;
  enum proc_state state;
  enum proc_type type;
  char name[32];

  u16 pid;
  bool killed;
};

void proc_init(void);
void sched_start(void);
