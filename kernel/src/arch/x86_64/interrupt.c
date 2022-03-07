//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#include "defs.h"
#include "arch/x86.h"
#include "arch/x86/interrupt.h"
#include "arch/x86/system.h"

#pragma pack(push, 0)
typedef struct {
  uint16_t size;
  void* idt;
} idtr_t;
#pragma pack(pop)

__attribute__((aligned(8)))
static idt64_t idt;

__attribute__((interrupt))
void the_handler(void* _unused_) {
    panic("breakpoint\n");
}

void interrupt_init(void) {
    memset(&idt, 0, sizeof(idt));
    idt.entries[IDT_ENTRY_BP] = mk_idt_entry(the_handler, 0, IDT_GATE_TYPE_INTERRUPT_TRAP, GDT_KERNEL_CODE);
    idt.entries[IDT_ENTRY_DOUBLE_FAULT] = mk_idt_entry(the_handler, 0, IDT_GATE_TYPE_INTERRUPT_TRAP, GDT_KERNEL_CODE);
    load_idt(&idt);
    kprintf("idt loaded\n");
}

void load_idt(idt64_t *idt) {
    volatile idtr_t idtr = {
            255,
            idt
    };
    asm volatile("lidt (%0)" : : "r"(&idtr));
}