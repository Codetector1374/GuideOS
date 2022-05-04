//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#ifndef GUIDE_OS_INTERRUPT_H
#define GUIDE_OS_INTERRUPT_H

#include "types.h"
#include "interrupt_vectors.h"

#define IDT_OFFSET_1(ptr)   ((size_t)(ptr) & 0xFFFF)
#define IDT_OFFSET_2(ptr)   (((size_t)(ptr) >> 16) & 0xFFFF)
#define IDT_OFFSET_3(ptr)   (((size_t)(ptr) >> 32) & 0xFFFFFFFF)

#pragma pack(push, 1)
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
#pragma pack(pop)

typedef void (*int_handler_t)(void*);

typedef struct trapframe {
  uint64_t rax;
  uint64_t rcx;
  uint64_t rdx;
  uint64_t rbx;
  uint64_t rsi;
  uint64_t rdi;
  uint64_t rbp;
  uint64_t r8;
  uint64_t r9;
  uint64_t r10;
  uint64_t r11;
  uint64_t r12;
  uint64_t r13;
  uint64_t r14;
  uint64_t r15;
  uint64_t trap_no;
  uint64_t error_code;
  uint64_t rip;
  uint64_t cs;
  uint64_t flags;
  uint64_t rsp;
  uint64_t ss;
} trapframe_t;

#define IDT_GATE_FLAGS_INT      0xE
#define IDT_GATE_FLAGS_TRAP     0xF
#define IDT_GATE_FLAGS_DPL_USER (0x3U << 5U)
#define IDT_GATE_FLAGS_DPL_KRNL (0x0U << 5U)
#define IDT_GATE_FLAGS_P        0x80

void handle_interrupt(trapframe_t *tf);
__attribute__((noreturn)) void return_from_isr(void);

inline
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

// lapic.c
u8   lapic_id(void);
void lapic_init(void);
void lapic_eoi(void);

// x86_pic.c
void pic_init(void);

// ioapic.c
void ioapic_init(void);
void ioapic_unmask(u8 irq, u8 vector, u8 apicid);
void ioapic_mask(u8 irq);

#endif //GUIDE_OS_INTERRUPT_H
