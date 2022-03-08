//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#ifndef GUIDE_OS_INTERRUPT_VECTORS_H
#define GUIDE_OS_INTERRUPT_VECTORS_H

#define IDT_ENTRY_DIV_BY_ZERO           0x0
#define IDT_ENTRY_DEBUG                 0x1
#define IDT_ENTRY_NMI                   0x2
#define IDT_ENTRY_BP                    0x3
#define IDT_ENTRY_OF                    0x4

#define IDT_ENTRY_INV_OPCODE            0x6
#define IDT_ENTRY_DOUBLE_FAULT          0x8

#ifndef __ASSEMBLER__
void vector_bp();
#endif

#endif //GUIDE_OS_INTERRUPT_VECTORS_H
