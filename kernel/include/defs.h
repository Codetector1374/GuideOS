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

#define MIN(a, b)               ((a) < (b) ? (a) : (b))

// Arch specific implementaiton
void panic(char *s);
bool disable_interrupt(void);
void enable_interrupt();
void wait_for_interrupt(void);

// cpu.c
void push_int_disable(void);
void pop_int_disable(void);
/**
 * @brief get the current cpu id between 0 and NCPU
 * @note must be called with interrupt disabled
 * 
 * @return u16 
 */
u16 cpu_id(void);
/**
 * @brief get a pointer to the current cpu strcture.
 * @note must be called with interrupt disabled
 * 
 * @return struct cpu* 
 */
struct cpu* cur_cpu(void);


// kmalloc.c
void *kmalloc(size_t size);
void *krealloc(void *ptr, size_t size);
void *kcalloc(size_t nmemb, size_t size);
void  kfree(void *ptr);


#endif //GUIDE_OS_DEFS_H
