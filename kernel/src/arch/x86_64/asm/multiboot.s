.intel_syntax noprefix

.section .multiboot, "a"
.align 8
multiboot_start:
.long 0xE85250D6
.long 0
.long (multiboot_end - multiboot_start)
.long 0x100000000 - (0xe85250d6 + 0 + (multiboot_end - multiboot_start))

# end tag
.short 0
.short 0
.long 8

multiboot_end:
