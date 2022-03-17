#include "defs.h"
#include "mem/boot_alloc.h"
#include "arch/x86.h"
#include "arch/x86/msr.h"
#include "arch/x86/interrupt.h"

__attribute__((noreturn))
void kmain(void) {
  boot_alloc_init(kern_end, 16 * 1024 * 1024); // Feed 16M into the boot_alloc
  console_init();
  kprintf("..kern..\n");
  interrupt_init();
  lapic_init();


  asm volatile("int3");
  kprintf("past breakpoint\n");

  for (;;) {
    wait_for_interrupt();
  }
}