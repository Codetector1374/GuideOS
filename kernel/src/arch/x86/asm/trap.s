.intel_syntax noprefix
.section .text.vectors
.code64

.extern handle_interrupt
.globl trap_enter_all

trap_enter_all:
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rbp
    push rdi
    push rsi
    push rbx
    push rdx
    push rcx
    push rax
    # Setup Call
    mov rdi, rsp
    call handle_interrupt
    pop rax
    pop rcx
    pop rdx
    pop rbx
    pop rsi
    pop rdi
    pop rbp
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
    add rsp, 16 # Pop trap_no and fake error_code
    iretq