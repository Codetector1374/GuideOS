//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#include "defs.h"
#include "arch/x86.h"
#include "arch/x86/interrupt.h"
#include "arch/x86/system.h"
#include "systick.h"

#pragma pack(push, 1)
typedef struct {
  uint16_t size;
  void *idt;
} idtr_t;
#pragma pack(pop)
_Static_assert(sizeof(idtr_t) == 10, "lol");

__attribute__((aligned(8)))
static idt64_t idt;

extern const uint64_t vectors[];

void handle_interrupt(trapframe_t *tf) {
  if (tf->trap_no == IDT_ENTRY_DOUBLE_FAULT) {
    kprintf("DF rip = %p\n", tf->rip);
    panic("A double fault has occurred");
  }

  switch (tf->trap_no) {
    case IDT_ENTRY_IRQ_SPURIOUS:
      lapic_eoi();
      return;
    case IDT_ENTRY_BP:
      kprintf("breakpoint at pc: %p\n", tf->rip);
      return;
    case IDT_ENTRY_IRQ_0:
      systick_increment();
      lapic_eoi();
      return;
    case IDT_ENTRY_IRQ_COM1:
      kprintf("com1 interrupt\n");
      lapic_eoi();
      return;
    case IDT_ENTRY_PAGE_FAULT:
      size_t faulting_addr = rcr2();
      kprintf("pg fault on %p at \nrip=%p\n", faulting_addr, tf->rip);
      panic("pgfault");
    default:
      kprintf("unhandled %d\n", tf->trap_no);
      panic("interrupt_handler");
      return;
  }
}

void interrupt_init(void) {
  memset(&idt, 0, sizeof(idt));
  for (int i = 0; i < 256; ++i) {
    idt.entries[i] = mk_idt_entry((int_handler_t) vectors[i], 
      0, 
      IDT_GATE_FLAGS_P | IDT_GATE_FLAGS_DPL_KRNL | IDT_GATE_FLAGS_INT, 
      GDT_KERNEL_CODE * 8);
  }

  load_idt(&idt);
}

void load_idt(idt64_t *idt) {
  volatile idtr_t idtr = {
          sizeof(idt64_t) - 1,
          idt
  };
  volatile idtr_t *pIDTR = &idtr;
  asm volatile("lidt (%0)" : : "r"(pIDTR));
}