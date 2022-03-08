#include "defs.h"
#include "mem/boot_alloc.h"
#include "arch/x86.h"
#include "arch/x86/msr.h"
#include "arch/x86/interrupt.h"

__attribute__((noreturn))
void kmain(void) {
    kprintf("..kern..\n");
    interrupt_init();
    boot_alloc_init(kern_end, 16*1024*1024); // Feed 16M into the boot_alloc

//    u64 apic_msr = rdmsr(IA32_APIC_BASE_MSR);
//    kprintf("APIC BASE = %x\n", apic_msr);


    asm volatile("int3");
    kprintf("past breakpoint\n");

    for(;;) {
        wait_for_interrupt();
    }
}