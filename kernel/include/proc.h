#pragma once

#include "param.h"
#include "types.h"
#include "mem/vm.h"
#include "arch/x86/vmm.h"
#include "arch/x86/interrupt.h"
#include "spinlock.h"

enum proc_state { UNUSED, CREATION, SLEEPING, RUNNABLE, RUNNING, ZOMBIE};
enum proc_type { KERNEL, USER };

struct proc_context {
  uint64_t rbx;
  uint64_t rbp;
  uint64_t r12;
  uint64_t r13;
  uint64_t r14;
  uint64_t r15;
  uint64_t rip;
};

struct proc {
  pml4e_t *pgtable;  // Root Page Table
  trapframe_t *tf;
  struct proc_context *ctx;
  void *kstack;
  void *kstack_top;
  enum proc_state state;
  enum proc_type type;
  char name[32];

  void* chan;

  uint32_t int_disable_layer;

  u16 pid;
  bool killed;
  bool int_enabled;
};

extern struct proc idle_proc[NCPU];

void proc_init(void);
struct proc* proc_alloc(void);
void proc_kern_init(struct proc* p);

void sched_init(void);
void sched_add(struct proc* proc);
void sched_start(void);
void sched_create_kproc(struct proc* proc, void (*func)(void));
void sched_ctxsw(struct proc_context **old, const struct proc_context *new);
void sched_kentry(void);
void sched_switch_out(void);
void sched_yield(void);
void sched_sleep(void* chan, spinlock_t *lk);
void sched_wakeup(void* chan);
