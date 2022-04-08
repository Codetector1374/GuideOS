#include "syscall.h"

#include "defs.h"

void handle_syscall(trapframe_t *tf) {
  u64 syscallno = tf->rax;
  kprintf("syscall %d\n", syscallno);
  
}