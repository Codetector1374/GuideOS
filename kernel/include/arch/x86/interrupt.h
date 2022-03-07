//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#ifndef GUIDE_OS_INTERRUPT_H
#define GUIDE_OS_INTERRUPT_H

typedef struct InterruptGate64 {
  uint16_t offset_1;        // offset bits 0..15
  uint16_t selector;        // a code segment selector in GDT or LDT
  uint8_t  ist;             // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
  uint8_t  type_attributes; // gate type, dpl, and p fields
  uint16_t offset_2;        // offset bits 16..31
  uint32_t offset_3;        // offset bits 32..63
  uint32_t zero;            // reserved
} interrupt_gate_64_t;

typedef struct IDT64 {
  interrupt_gate_64_t entries[256];
} idt64_t;


#define IDT_GATE_TYPE_INTERRUPT_GATE    0x8E
#define IDT_GATE_TYPE_INTERRUPT_TRAP    0x8F

typedef void (*int_handler_t)(void);

static interrupt_gate_64_t mk_idt_date(int_handler_t handler_fn);

#endif //GUIDE_OS_INTERRUPT_H
