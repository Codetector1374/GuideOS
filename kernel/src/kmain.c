#include "defs.h"
#include "mem/boot_alloc.h"
#include "mem/kmalloc.h"
#include "mem/vm.h"
#include "mem/page_alloc.h"
#include "arch/x86/interrupt.h"
#include "arch/x86/instructions.h"
#include "systick.h"
#include "cpu.h"
#include "device/uart.h"
#include "proc.h"

__attribute__((noreturn)) void mpmain(void);

__attribute__((noreturn))
void kmain(void)
{
  boot_alloc_init(kern_end, 64 * 1024 * 1024); // Feed 64M into the boot_alloc
  vmm_init();
  pgalloc_init(4UL * 1024 * 1024 * 1024); // TODO: remove hard code 4G

  // Interrupt related
  lapic_init();       // LAPIC
  cpu_bsp_init();     // We need to do this early so we can have locks
  interrupt_init();   // IDT
  pic_init();         // Disable PICs
  ioapic_init();      // Setup 1 IOAPIC

  // Locks should be ready by this point
  void *bootalloc_avail = boot_alloc_disable();
  kmalloc_init();
  pgalloc_free_range(bootalloc_avail, P2KV(64 * 1024 * 1024));

  // System Devices
  console_init();
  systick_init();

  // Initialize pTable
  proc_init();

  mpmain();
}

void mpmain() 
{
  // ioapic_unmask(4, IDT_ENTRY_IRQ_0 + 4, lapic_id());
  kprintf("cpu %u started\n", cpu_id());
  // sti();
  sched_start();
  kprintf("sched failed to start");
  for (;;) {
    wait_for_interrupt();
  }
}
