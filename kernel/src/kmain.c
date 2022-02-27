#include "vm.h"
#include "console.h"

__attribute__((noreturn))
void kmain(void) {
    console_init();
    putchar('A');
    for(;;) {}
}