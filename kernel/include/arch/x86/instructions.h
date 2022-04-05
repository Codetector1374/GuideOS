//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#ifndef GUIDE_OS_INSTRUCTIONS_H
#define GUIDE_OS_INSTRUCTIONS_H

#include "types.h"

#define RFLAGS_IF           0x00000200

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

static inline size_t
read_flags()
{
    size_t flags;
    __asm__ volatile("pushf; pop %0" : "=r" (flags));
    return flags;
}

static inline size_t
rcr2(void)
{
  size_t val;
  __asm__ volatile("mov %%cr2,%0" : "=r" (val));
  return val;
}

static inline void
lcr3(size_t cr3)
{
  __asm__ volatile("mov %0, %%cr3" : : "r"(cr3) : "memory");
}

static inline u64
rdmsr(u32 msr_addr) {
    u32 msrh, msrl;
    __asm__ volatile("rdmsr"
    : "=d"(msrh), "=a"(msrl)
    : "c"(msr_addr)
    );
    return (((u64)msrh) << 32) | msrl;
}

static inline void
wrmsr(u32 msr_addr, u64 msr_data) {
    u32 msrh, msrl;
    msrh = msr_data >> 32;
    msrl = (u32) msr_data;
    __asm__ volatile("wrmsr"
    :
    : "c"(msr_addr), "a"(msrl), "d"(msrh)
    );
}

#endif //GUIDE_OS_INSTRUCTIONS_H
