.intel_syntax noprefix

.extern kmain
.globl kentry
.globl raw_entry


.code32
.section .raw_init, "ax"
raw_entry:
    mov     eax, 0xb0bacafe
    mov     ebx, 0
    jmp     kentry

.section .bootstrap, "ax"
.align 8
kentry:
    cli
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
    mov     rax, 0xFFFF800000000000
    add     rax, OFFSET gdt64
    mov     [gdtdesc_addr], rax
    movabs  rax, OFFSET gdtdesc
    mov     rcx, 0xFFFF800000000000
    add     rax, rcx
    lgdt    [rax]

    mov     rdi, DWORD [rsp]
    mov     rsi, DWORD [rsp + 4]
    mov     rbp, 0xFFFF800000000000
    mov     rsp, 0xFFFF800000000000 + 0x10000
    movabs  rax, OFFSET kmain
    call    rax

halt:
    hlt
    jmp halt

gdt64:
    # Null Entry
    .quad   0

    .quad (1<<43) | (1<<44) | (1<<47) | (1<<53)

    .quad (1<<44) | (1<<47) | (1<<53)

gdtdesc:
    .short   (gdtdesc - gdt64 - 1)
gdtdesc_addr:
    .quad   gdt64

.section .bootstrap.data, "wa"
.align 4096
pml4:
.fill 512, 8, 0

.align 4096
pdp:
.quad (0x80 | 0x2 | 0x1)
.fill 511, 8, 0
