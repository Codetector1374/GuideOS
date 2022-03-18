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
  interrupt_init();   // IDT
  lapic_init();   // LAPIC
  pic_init();   // Disable PICs
  // System Devices
  systick_init();
  console_init();

  mpmain();
}

void mpmain() 
{
  cpu_bsp_init();
  kprintf("cpu %u started...\n", cpu_id());
  sti();
  for (;;) {
    kprintf("idle %lx\n", systick());
    wait_for_interrupt();
  }
}
