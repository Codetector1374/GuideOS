#include "defs.h"
#include "mem/boot_alloc.h"
#include "mem/vm.h"
#include "arch/x86/interrupt.h"
#include "arch/x86/instructions.h"
#include "systick.h"
#include "cpu.h"

__attribute__((noreturn)) void mpmain(void);

__attribute__((noreturn))
void kmain(void)
{
  boot_alloc_init(kern_end, 4 * 1024 * 1024); // Feed 4M into the boot_alloc
  vmm_init();
  // Interrupt related
  lapic_init();       // LAPIC
  cpu_bsp_init();     // We need to do this early so we can have locks
  interrupt_init();   // IDT
  pic_init();         // Disable PICs
  ioapic_init();      // Setup 1 IOAPIC
  // System Devices
  console_init();
  systick_init();

  mpmain();
}

void mpmain() 
{
  kprintf("cpu %u started...\n", cpu_id());
  sti();
  for (;;) {
    wait_for_interrupt();
  }
}
