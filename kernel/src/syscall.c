#include "syscall.h"
#include "syscall_number.h"
#include "proc.h"
#include "defs.h"


static uint64_t sys_yield(trapframe_t *tf) {
  struct proc* p = curproc();
  push_int_disable();
  if (p->state == RUNNING)
    p->state = RUNNABLE;
  pop_int_disable();
  return 0;
}

static uint64_t sys_con_read(trapframe_t *tf) {
  struct proc* p = curproc();
  push_int_disable();
  if (p->state == RUNNING)
    p->state = RUNNABLE;
  pop_int_disable();
  return 0;
}

void handle_syscall(trapframe_t *tf) {
  u64 syscallno = tf->rax;
  switch(syscallno) {
    case SYS_YIELD: tf->rax = sys_yield(tf); break;
    case SYS_CON_READ: tf->rax = sys_con_read(tf); break;
    default: kprintf("unknown syscall %d\n", syscallno); break;
  }

}
