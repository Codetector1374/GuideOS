//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#ifndef GUIDE_OS_INTERRUPT_VECTORS_H
#define GUIDE_OS_INTERRUPT_VECTORS_H

#define IDT_ENTRY_DIV_BY_ZERO           0
#define IDT_ENTRY_DEBUG                 1
#define IDT_ENTRY_NMI                   2
#define IDT_ENTRY_BP                    3
#define IDT_ENTRY_OF                    4

#define IDT_ENTRY_INV_OPCODE            6
#define IDT_ENTRY_DOUBLE_FAULT          8

#define IDT_ENTRY_PAGE_FAULT            14

#define IDT_ENTRY_IRQ_0                 32
#define IDT_ENTRY_IRQ_TIMER             (IDT_ENTRY_IRQ_0 + 0)
#define IDT_ENTRY_IRQ_COM1              (IDT_ENTRY_IRQ_0 + 4)
#define IDT_ENTRY_IRQ_ERROR             (IDT_ENTRY_IRQ_0 + 19)
#define IDT_ENTRY_IRQ_SPURIOUS          (IDT_ENTRY_IRQ_0 + 31)

#endif //GUIDE_OS_INTERRUPT_VECTORS_H
