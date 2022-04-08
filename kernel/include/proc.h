#pragma once

#include "param.h"
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

  void* chan;

  u16 pid;
  bool killed;
};

extern struct proc idle_proc[NCPU];

void proc_init(void);
struct proc* proc_alloc(void);

void sched_init(void);
void sched_add(struct proc* proc);
void sched_start(void);
void sched_create_kproc(struct proc* proc, void (*func)(void));
void sched_switch(trapframe_t *tf);
