#include "types.h"
#include "defs.h"
#include "arch/x86/interrupt.h"

#define IOAPIC  0xFEC00000

#define IOAPIC_REG_ID       0x00
#define IOAPIC_REG_VER      0x01
#define IOAPIC_REG_TABLE    0x10

//
// The redirection table starts at REG_TABLE and uses
// two registers to configure each interrupt.
// The first (low) register in a pair contains configuration bits.
// The second (high) register contains a bitmask telling which
// CPUs can serve that interrupt.
//
#define INT_DISABLED   0x00010000  // Interrupt disabled
#define INT_LEVEL      0x00008000  // Level-triggered (vs edge-)
#define INT_ACTIVELOW  0x00002000  // Active low (vs high)
#define INT_LOGICAL    0x00000800  // Destination is CPU id (vs APIC ID)

volatile struct ioapic *ioapic;

// IO APIC MMIO structure: write reg, then read or write data.
struct ioapic {
  u32 reg;
  u32 pad[3];
  u32 data;
};

static u32
ioapicread(int reg)
{
  ioapic->reg = reg;
  return ioapic->data;
}

static void
ioapicwrite(int reg, u32 data)
{
  ioapic->reg = reg;
  ioapic->data = data;
}

void
ioapic_init(void)
{
  int i, id, maxintr;

  ioapic = (volatile struct ioapic*)P2KV(IOAPIC);
  maxintr = (ioapicread(IOAPIC_REG_VER) >> 16) & 0xFF;

  // Mark all interrupts edge-triggered, active high, disabled,
  // and not routed to any CPUs.
  for(i = 0; i <= maxintr; i++){
    ioapicwrite(IOAPIC_REG_TABLE+2*i, INT_DISABLED | (IDT_ENTRY_IRQ_0 + i));
    ioapicwrite(IOAPIC_REG_TABLE+2*i+1, 0);
  }
}

void
ioapic_enable(u8 irq, u8 apicid)
{
  // Mark interrupt edge-triggered, active high,
  // enabled, and routed to the given cpunum,
  // which happens to be that cpu's APIC ID.
  ioapicwrite(IOAPIC_REG_TABLE+2*irq, IDT_ENTRY_IRQ_0 + irq);
  ioapicwrite(IOAPIC_REG_TABLE+2*irq+1, (u32)apicid << 24);
}
