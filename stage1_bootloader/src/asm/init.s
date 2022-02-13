# set intel syntax with no prefix on register nor immediate
.intel_syntax noprefix

#
# We place the following code in the .text.init section so we can place
# it at a location we want using the linker script (0x7c00)
#
.section .text.init

# Set to 16-bit code mode. Since we are going to start in 16-bit real mode.
.code16
.globl start

start:
    #
    # This jump is not strictly necessary, but some BIOS will start you at 0x07C0:0000
    # which is in fact the linear address as 0x0:7C00, but the range of jump will be
    # different. We will unify that with this long jump.
    #
    jmp 0:true_start

.section .text

true_start:
    cli     # disable interrupts

    # We zero the segment registers
    xor     ax, ax
    mov     ds, ax
    mov     es, ax
    mov     ss, ax

    # Clear Screen and set video mode to 2
    mov     ah, 0
    mov     al, 2
    int     0x10

    # Print Hello World Using BIOS
    xor ax, ax
    mov es, ax
    xor bh, bh
    lea bp, hello

    mov ah, 0x13
    mov bl, 0x4 # red foreground
    mov al, 1
    mov cx, [hello_len]
    mov dh, 0 # y
    mov dl, 0 # x
    int 0x10


here:
    jmp here

hello: .ascii "Hello World"
        .byte 0
hello_len: .word $-hello

