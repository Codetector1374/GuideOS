.intel_syntax noprefix

.extern kmain


.code32
.section .text.raw_init
raw_entry:
    mov     eax, 0xb0bacafe
    mov     ebx, 0
    jmp     _start

.code32
.section .text.bootstrap

.globl _start
_start:
    mov esp, 0x10000
    push ebx
    push eax

# Build Page Tables
    mov eax, OFFSET pdp
    or eax, 0b11
    mov [pml4], eax
    mov [pml4 + (256 * 8)], eax

# Enable Paging / Long Mode
    mov eax, OFFSET pml4
    mov cr3, eax

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    lgdt    [gdtdesc]
    jmp     0x8:long_start

# Long mode
.code64

long_start:
    xor     ax, ax
    mov     ds, ax
    mov     es, ax
    mov     ss, ax
    mov     fs, ax
    mov     gs, ax

    mov     rdi, DWORD [rsp]
    mov     rsi, DWORD [rsp + 4]
    movabs  rax, OFFSET kmain
    call    rax

halt:
    hlt
    jmp halt

gdt64:
    # Null Entry
    .short   0, 0
    .byte    0,0,0,0

    # Code Segment
    .short 0xFFFF, 0
    .byte 0
    .byte 0b10011010
    .byte 0b11101111
    .byte 0

    # Data Segment
    .short 0xFFFF, 0
    .byte 0
    .byte 0b10010010
    .byte 0b11101111
    .byte 0

gdtdesc:
    .short   (gdtdesc - gdt64 - 1)
    .quad   gdt64

.align 4096
pml4:
.fill 512, 8, 0

.align 4096
pdp:
.quad (0x80 | 0x2 | 0x1)
.fill 511, 8, 0
