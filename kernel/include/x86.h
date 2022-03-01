//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#ifndef GUIDE_OS_X86_H
#define GUIDE_OS_X86_H

#include "types.h"

static inline void
cli(void)
{
    __asm__ volatile("cli");
}

static inline void
hlt(void)
{
    __asm__ volatile("hlt");
}

static inline void
sti(void)
{
    __asm__ volatile("sti");
}

static inline u8
inb(u16 port)
{
    u8 data;

    __asm__ volatile("in %1,%0" : "=a" (data) : "d" (port));
    return data;
}

static inline void
insl(int port, void *addr, int cnt)
{
    __asm__ volatile("cld; rep insl" :
    "=D" (addr), "=c" (cnt) :
    "d" (port), "0" (addr), "1" (cnt) :
    "memory", "cc");
}

static inline void
outb(u16 port, u8 data)
{
    __asm__ volatile("out %0,%1" : : "a" (data), "d" (port));
}

static inline void
outw(u16 port, u16 data)
{
    __asm__ volatile("out %0,%1" : : "a" (data), "d" (port));
}

static inline void
outsl(int port, const void *addr, int cnt)
{
    __asm__ volatile("cld; rep outsl" :
    "=S" (addr), "=c" (cnt) :
    "d" (port), "0" (addr), "1" (cnt) :
    "cc");
}

static inline void
stosb(void *addr, int data, int cnt)
{
    __asm__ volatile("cld; rep stosb" :
    "=D" (addr), "=c" (cnt) :
    "0" (addr), "1" (cnt), "a" (data) :
    "memory", "cc");
}

static inline void
stosl(void *addr, int data, int cnt)
{
    __asm__ volatile("cld; rep stosl" :
    "=D" (addr), "=c" (cnt) :
    "0" (addr), "1" (cnt), "a" (data) :
    "memory", "cc");
}

#endif //GUIDE_OS_X86_H
