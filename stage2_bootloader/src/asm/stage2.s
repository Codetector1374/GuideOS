# set intel syntax with no prefix on register nor immediate
.intel_syntax noprefix

# Set to 16-bit code mode. Since we are going to start in 16-bit real mode.
.code16
.globl stage2
.globl cputs

.extern c_entry

.section .text.start

stage2:
    cli     # disable interrupts
    cld

    # We zero the segment registers
    xor     ax, ax
    mov     ds, ax
    mov     es, ax
    mov     ss, ax

    mov     sp, OFFSET stage2   # SP is loaded with 0x7E00, we can use all that memory below code as stack.

# We need to store our drive number onto the stack (DL)
    push   dx   # Let's just push the entire DX register

    lea     si, loading_string
    call    print_string

    call    c_entry

end:
    hlt
    jmp     end

cputs:
    push    bp
    mov     bp, sp
    push    si
    mov     si, [bp+6]
    call print_string
    pop     si
    pop     bp
    ret

print_string:
    push    ax
    push    si
    push    bx
    xor     bx, bx
    mov     ah, 0xe       # int 10h 'print char' function

repeat:
    lods    al, [si]   # Get character from string
    test    al, al
    je      done         # If char is zero, end of string
    int     0x10           # Otherwise, print it
    jmp     repeat
done:
    pop bx
    pop si
    pop ax
    ret

loading_string:
    .ascii "In Stage 2\r\n"
    .byte 0

disk_address_block:
    .byte       0x10    # length of this block
    .byte       0x0     # reserved
    .short      64      # number of blocks = 32k/512b = 64
    .long       0x07E00000  # Target memory address
    .quad       1       # Starting Disk block 1, since we just need to skip the boot sector.

