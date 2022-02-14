/* Linker Script for the Stage 2 Bootloader */

MEMORY {
    stage2 : ORIGIN = 0x7E00, LENGTH = 32k
}

ENTRY(stage2)
SECTIONS {
    text : {
        *(.text .text*)
    } >stage2
    .rodata : {
        *(.rodata)
    } >stage2

    .bss : {
        *(.bss)
    } >stage2
    .data : {
        *(.data)
    } >stage2
    /DISCARD/ : {
        *(.debug*)
    }
}
