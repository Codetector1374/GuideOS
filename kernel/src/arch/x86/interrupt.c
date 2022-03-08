//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#include "defs.h"
#include "arch/x86.h"
#include "arch/x86/interrupt.h"
#include "arch/x86/system.h"

#pragma pack(push, 1)
typedef struct {
  uint16_t size;
  void* idt;
} idtr_t;
#pragma pack(pop)
_Static_assert(sizeof(idtr_t) == 10, "lol");

__attribute__((aligned(8)))
static idt64_t idt;

void handle_interrupt(trapframe_t *tf)
{
    if (tf->int_num == IDT_ENTRY_BP) {
        kprintf("breakpoint at pc: %p\n", tf->rip);
        return;
    }
    panic("interrupt_handler");
}

__attribute__((interrupt))
void double_fault(void *_unused_) {
    panic("A double fault has occurred");
}

void interrupt_init(void) {
    memset(&idt, 0, sizeof(idt));
    idt.entries[IDT_ENTRY_BP] = mk_idt_entry(vector_bp, 0, IDT_GATE_TYPE_INTERRUPT_TRAP, GDT_KERNEL_CODE);
    idt.entries[IDT_ENTRY_DOUBLE_FAULT] = mk_idt_entry(double_fault, 0, IDT_GATE_TYPE_INTERRUPT_TRAP, GDT_KERNEL_CODE);
    load_idt(&idt);
    kprintf("idt loaded\n");
}

void load_idt(idt64_t *idt) {
    volatile idtr_t idtr = {
            255,
            idt
    };
    volatile idtr_t *pIDTR = &idtr;
    asm volatile("lidt (%0)" : : "r"(pIDTR));
}