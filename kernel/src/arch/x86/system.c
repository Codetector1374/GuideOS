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
    return (flags & RFLAGS_IF) > 0;
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

void tss_setup(struct cpu* cpu) 
{
    memset(&cpu->tss, 0, sizeof(cpu->tss));
}

void gdt_setup(struct cpu *cpu)
{
    volatile struct gdtr gdtr;
    struct gdt* gdt = &cpu->gdt;
    memset(gdt, 0, sizeof(struct gdt));
    // KCODE
    gdt->seg_descs[GDT_KERNEL_CODE].type = GDT_DESC_TYPE_CODE;
    gdt->seg_descs[GDT_KERNEL_CODE].dpl = 0;
    gdt->seg_descs[GDT_KERNEL_CODE].p = 1;
    gdt->seg_descs[GDT_KERNEL_CODE].flags = GDT_DESC_FLAGS_L | GDT_DESC_FLAGS_G;
    // KDATA
    gdt->seg_descs[GDT_KERNEL_DATA] = gdt->seg_descs[GDT_KERNEL_CODE];
    gdt->seg_descs[GDT_KERNEL_DATA].type = GDT_DESC_TYPE_DATA;

    // UCODE
    gdt->seg_descs[GDT_USER_CODE] = gdt->seg_descs[GDT_KERNEL_CODE];
    gdt->seg_descs[GDT_USER_CODE].dpl = 3;
    // UDATA
    gdt->seg_descs[GDT_USER_DATA] = gdt->seg_descs[GDT_KERNEL_DATA];
    gdt->seg_descs[GDT_USER_DATA].dpl = 3;
    
    struct task_state_segment* tss = &cpu->tss;
    size_t tss_addr = (size_t) tss;
    gdt->tss_entry.type = GDT_DESC_TYPE_TSS;
    gdt->tss_entry.p = 1;
    gdt->tss_entry.dpl = 0;
    gdt->tss_entry.flags = GDT_DESC_FLAGS_L;
    gdt->tss_entry.limit_15_0 = sizeof(struct task_state_segment) & 0xFFFF;
    gdt->tss_entry.limit_19_16 = (sizeof(struct task_state_segment) >> 16U) & 0xF;
    gdt->tss_entry.base_15_0  = (tss_addr >> 0U ) & 0xFFFFU;
    gdt->tss_entry.base_23_16 = (tss_addr >> 16U) & 0xFFU;
    gdt->tss_entry.base_31_24 = (tss_addr >> 24U) & 0xFFU;
    gdt->tss_entry.base_63_32 = (tss_addr >> 32U) & 0xFFFFFFFFU;

    gdtr.gdt = gdt;
    gdtr.size = sizeof(struct gdt) - 1;
    asm volatile("lgdt (%0)" ::"r"(&gdtr));
}
