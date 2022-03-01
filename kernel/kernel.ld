/* Kernel Linker Script */
ENTRY(kentry)
SECTIONS {
    . = 1M;
    .raw_init : {
        KEEP(*(.raw_init))
    }
    .multiboot BLOCK(8) : {
        KEEP(*(.multiboot))
    }
    .bootstrap : {
        *(.bootstrap)
    }
    .bootstrap.data BLOCK(4k) : {
        *(.bootstrap.data)
    }
    kernel_start_pa = .;
    .kernel (0xFFFF800000000000 + kernel_start_pa) : AT (kernel_start_pa) {
        *(.text .text.*)
        *(.rodata .rodata.*)
        *(.data .data.*)
        *(.bss .bss.*)
    }
    . = ALIGN(4k);
    kern_end = .;
    /DISCARD/ : {
        *(.eh_frame)
    }
}
