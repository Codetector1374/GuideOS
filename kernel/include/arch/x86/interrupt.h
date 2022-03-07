//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#ifndef GUIDE_OS_INTERRUPT_H
#define GUIDE_OS_INTERRUPT_H

#include "types.h"

#define IDT_OFFSET_1(ptr)   ((size_t)(ptr) & 0xFFFF)
#define IDT_OFFSET_2(ptr)   (((size_t)(ptr) >> 16) & 0xFFFF)
#define IDT_OFFSET_3(ptr)   (((size_t)(ptr) >> 32) & 0xFFFFFFFF)

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

typedef void (*int_handler_t)(void*);

#define IDT_GATE_TYPE_INTERRUPT_GATE    0x8E
#define IDT_GATE_TYPE_INTERRUPT_TRAP    0x8F

#define IDT_ENTRY_DIV_BY_ZERO           0x0
#define IDT_ENTRY_DEBUG                 0x1
#define IDT_ENTRY_NMI                   0x2
#define IDT_ENTRY_BP                    0x3
#define IDT_ENTRY_OF                    0x4

#define IDT_ENTRY_INV_OPCODE            0x6
#define IDT_ENTRY_DOUBLE_FAULT          0x8

static inline
interrupt_gate_64_t
mk_idt_entry(int_handler_t handler_fn, uint8_t ist, uint8_t type_attr, uint8_t cs)
{
    return (interrupt_gate_64_t) {
        .offset_1 = IDT_OFFSET_1(handler_fn),
        .offset_2 = IDT_OFFSET_2(handler_fn),
        .offset_3 = IDT_OFFSET_3(handler_fn),
        .ist = ist & 0x7,
        .type_attributes = type_attr,
        .selector = cs,
        .zero = 0
    };
}

void interrupt_init(void);

void load_idt(idt64_t *idt);

#endif //GUIDE_OS_INTERRUPT_H
