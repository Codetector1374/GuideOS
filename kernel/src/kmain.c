#include "defs.h"
#include "mem/boot_alloc.h"
#include "mem/vm.h"
#include "arch/x86/interrupt.h"
#include "arch/x86/instructions.h"
#include "systick.h"

__attribute__((noreturn))
void kmain(void) {
  systick_init();
  console_init();
  boot_alloc_init(kern_end, 16 * 1024 * 1024); // Feed 16M into the boot_alloc
  vmm_init();
  kprintf("..kern..\n");
  interrupt_init();
  lapic_init();
  pic_init();
  asm volatile("int3");
  sti();


  kprintf("past breakpoint\n");

  for (;;) {
    wait_for_interrupt();
  }
}