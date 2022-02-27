#include "defs.h"


__attribute__((noreturn))
void kmain(void) {
    kprintf("..kern..\n");
    kprintf("kern end at %p", kern_end);
    for(;;) {}
}