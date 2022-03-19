#pragma once

#include "types.h"
#include "arch/x86/system.h"

// Per-CPU state
struct cpu {
    u32 started;
    u32 int_disable_layer;
    u32 int_enabled;
    struct gdt gdt;
    struct proc* proc;
    u8 apicid;
};

void cpu_bsp_init(void);
