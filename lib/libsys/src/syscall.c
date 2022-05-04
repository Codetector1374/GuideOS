#include "types.h"
#include "usys.h"
#include "syscall_number.h"

uint64_t syscall0(uint64_t syscallNo) {
  uint64_t rtnval;
  asm volatile("int $0x80" : "=a"(rtnval) : "a"(syscallNo) : "cc", "memory");
  return rtnval;
}

uint64_t syscall1(uint64_t syscallNo, uint64_t arg0) {
  uint64_t rtnval;
  asm volatile("int $0x80" : "=a"(rtnval) : "a"(syscallNo), "rdi"(arg0) : "cc", "rdi", "memory");
  return rtnval;
}

// uint64_t syscall2(uint64_t, uint64_t, uint64_t);
// uint64_t syscall3(uint64_t, uint64_t, uint64_t, uint64_t);

void yield(void) {
  syscall0(SYS_YIELD);
}

int con_read(void) {
  return (int)syscall0(SYS_CON_READ);
}

