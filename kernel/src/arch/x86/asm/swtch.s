.intel_syntax noprefix

.section .text
.globl sched_ctxsw

# void sched_ctxsw(struct proc_context **old, const struct proc_context *new);
#                   rdi,                        rsi
sched_ctxsw:
  push r15
  push r14
  push r13
  push r12
  push rbp
  push rbx

  mov [rdi], rsp
  mov rsp, rsi

  pop rbx
  pop rbp
  pop r12
  pop r13
  pop r14
  pop r15
  ret