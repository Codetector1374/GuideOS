
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#include "defs.h"
#include "types.h"
#include "arch/x86/instructions.h"
#include "arch/x86/msr.h"
#include "arch/x86/interrupt.h"
#include "arch/x86/interrupt_vectors.h"

// Local APIC registers, divided by 4 for use as uint[] indices.
#define ID      (0x0020/4)   // ID
#define VER     (0x0030/4)   // Version
#define TPR     (0x0080/4)   // Task Priority
#define EOI     (0x00B0/4)   // EOI
#define SVR     (0x00F0/4)   // Spurious Interrupt Vector
#define   ENABLE     0x00000100   // Unit Enable
#define ESR     (0x0280/4)   // Error Status
#define ICRLO   (0x0300/4)   // Interrupt Command
#define   INIT       0x00000500   // INIT/RESET
#define   STARTUP    0x00000600   // Startup IPI
#define   DELIVS     0x00001000   // Delivery status
#define   ASSERT     0x00004000   // Assert interrupt (vs deassert)
#define   DEASSERT   0x00000000
#define   LEVEL      0x00008000   // Level triggered
#define   BCAST      0x00080000   // Send to all APICs, including self.
#define   BUSY       0x00001000
#define   FIXED      0x00000000
#define ICRHI   (0x0310/4)   // Interrupt Command [63:32]
#define TIMER   (0x0320/4)   // Local Vector Table 0 (TIMER)
#define   X1         0x0000000B   // divide counts by 1
#define   PERIODIC   0x00020000   // Periodic
#define PCINT   (0x0340/4)   // Performance Counter LVT
#define LINT0   (0x0350/4)   // Local Vector Table 1 (LINT0)
#define LINT1   (0x0360/4)   // Local Vector Table 2 (LINT1)
#define ERROR   (0x0370/4)   // Local Vector Table 3 (ERROR)
#define   MASKED     0x00010000   // Interrupt masked
#define TICR    (0x0380/4)   // Timer Initial Count
#define TCCR    (0x0390/4)   // Timer Current Count
#define TDCR    (0x03E0/4)   // Timer Divide Configuration


volatile u32 *lapic;

static void
lapicw(int index, int value)
{
  lapic[index] = value;
  lapic[ID];  // wait for write to finish, by reading
}

u8 lapic_id(void) {
  return (lapic[ID] >> 24U);
}

void lapic_eoi(void)
{
  if(lapic)
    lapicw(EOI, 0);
}

void lapic_init(void) {
  uint64_t apic_base_pa = rdmsr(IA32_APIC_BASE_MSR);
  if (!(apic_base_pa & IA32_APIC_BASE_MSR_ENABLED)) panic("apic disabled");
  lapic = (volatile u32 *) P2KV(apic_base_pa & IA32_APIC_BASE_MSR_BASE_ADDR_MSK);

  // Enable local APIC; set spurious interrupt vector.
  lapicw(SVR, ENABLE | IDT_ENTRY_IRQ_SPURIOUS);

  // The timer repeatedly counts down at bus frequency
  // from lapic[TICR] and then issues an interrupt.
  // TODO: calibrate TICR with external time
  lapicw(TDCR, X1);
  lapicw(TIMER, PERIODIC | IDT_ENTRY_IRQ_TIMER);
  lapicw(TICR, 10000000);

  // Disable logical interrupt lines.
  lapicw(LINT0, MASKED);
  lapicw(LINT1, MASKED);

  // Disable performance counter overflow interrupts
  // on machines that provide that interrupt entry.
  if(((lapic[VER]>>16) & 0xFF) >= 4)
    lapicw(PCINT, MASKED);

  // Map error interrupt to IRQ_ERROR.
  lapicw(ERROR, IDT_ENTRY_IRQ_ERROR);

  // Clear error status register (requires back-to-back writes).
  lapicw(ESR, 0);
  lapicw(ESR, 0);

  // Ack any outstanding interrupts.
  lapicw(EOI, 0);

  // Send an Init Level De-Assert to synchronise arbitration ID's.
  lapicw(ICRHI, 0);
  lapicw(ICRLO, BCAST | INIT | LEVEL);
  while(lapic[ICRLO] & DELIVS)
    ;

  // Enable interrupts on the APIC (but not on the processor).
  lapicw(TPR, 0);
}

