#include "defs.h"
#include "mem/boot_alloc.h"

__attribute__((noreturn))
void kmain(void) {
    kprintf("..kern..\n");
    boot_alloc_init(kern_end, 16*1024*1024); // Feed 16M into the boot_alloc


    for(;;) {}
}