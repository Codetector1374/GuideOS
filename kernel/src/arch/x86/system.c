//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#include "types.h"
#include "defs.h"
#include "arch/x86.h"
#include "arch/x86/system.h"
#include "cpu.h"

bool disable_interrupt()
{
    size_t flags = read_flags();
    cli();
    return (flags & FL_IF) > 0;
}

void enable_interrupt() {
    sti();
}

void wait_for_interrupt() {
    hlt();
}

#pragma pack(push, 1)
struct gdtr {
    u16 size;
    struct gdt* gdt;
};
_Static_assert(sizeof(struct gdtr) == 10, "gdtr size wrong");
#pragma pack(pop)

void gdt_setup(struct cpu *cpu)
{
    volatile struct gdtr gdtr;
    struct gdt* gdt = &cpu->gdt;
    memset(gdt, 0, sizeof(struct gdt));
    // KCODE
    gdt->seg_descs[GDT_KERNEL_CODE].type = GDT_DESC_TYPE_CODE;
    gdt->seg_descs[GDT_KERNEL_CODE].p = 1;
    gdt->seg_descs[GDT_KERNEL_CODE].flags = GDT_DESC_FLAGS_L;
    // KDATA
    gdt->seg_descs[GDT_KERNEL_DATA] = gdt->seg_descs[GDT_KERNEL_CODE];
    gdt->seg_descs[GDT_KERNEL_DATA].type = GDT_DESC_TYPE_DATA;

    // UCODE
    gdt->seg_descs[GDT_USER_CODE] = gdt->seg_descs[GDT_KERNEL_CODE];
    gdt->seg_descs[GDT_USER_CODE].dpl = 3;
    // UDATA
    gdt->seg_descs[GDT_USER_DATA] = gdt->seg_descs[GDT_USER_DATA];
    gdt->seg_descs[GDT_USER_DATA].dpl = 3;
    
    gdtr.gdt = gdt;
    gdtr.size = sizeof(struct gdt) - 1;
    asm volatile("lgdt (%0)" ::"r"(&gdtr));
}
