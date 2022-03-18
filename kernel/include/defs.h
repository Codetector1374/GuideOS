//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#ifndef GUIDE_OS_DEFS_H
#define GUIDE_OS_DEFS_H

#include "param.h"
#include "types.h"
#include "kprintf.h"
#include "mem/memlayout.h"
#include "string.h"
#include "cpu.h"

#define ROUNDUP_PWR2(x, sz)      (((x) + ((sz) - 1UL)) & ~((sz) - 1UL))
#define ROUNDDOWN_PWR2(x, sz)    ((x) & ~((sz) - 1))

// Arch specific implementaiton
void panic(char *s);
bool disable_interrupt(void);
void enable_interrupt();
void wait_for_interrupt(void);

// cpu.c
void push_int_disable(void);
void pop_int_disable(void);
u16 cpu_id(void);
struct cpu* cur_cpu(void);

#endif //GUIDE_OS_DEFS_H
