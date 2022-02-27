/* Kernel Linker Script */

SECTIONS {
    .text.raw_init 0x100000 : {
        KEEP(*(.text.raw_init))
    }
    .multiboot : {
        KEEP(*(.multiboot))
    }
    .text.bootstrap : {
        *(.text.bootstrap)
    }
    .kernel (0xFFFF800000000000 + ADDR(.text.bootstrap) + SIZEOF(.text.bootstrap)) : AT (LOADADDR(.text.bootstrap) + SIZEOF(.text.bootstrap)) {
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
