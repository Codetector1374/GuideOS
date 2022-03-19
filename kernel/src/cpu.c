#include "defs.h"
#include "arch/x86/instructions.h"
#include "arch/x86/interrupt.h"
#include "arch/x86/system.h"

static struct cpu cpus[NCPU];
static uint16_t ncpu = 0;

void cpu_bsp_init()
{
    ncpu = 1;
    struct cpu *bsp = &cpus[0];
    bsp->apicid = lapic_id();
    bsp->started = 1;
    bsp->int_disable_layer = 0;
    gdt_setup(bsp);
}

u16 cpu_id() {
    return cur_cpu() - cpus;
}

struct cpu* cur_cpu(void) {
    u8 apicid;
    if (read_flags() & FL_IF) {
        panic("cur_cpu with IF");
    }

    apicid = lapic_id();

    for (size_t i = 0; i < ncpu; i++)
    {
        if (cpus[i].apicid == apicid)
            return &cpus[i];
    }
    panic("unknown cpu");
}

void push_int_disable(void)
{
    bool enabled = disable_interrupt();
    struct cpu* c = cur_cpu();
    if (c->int_disable_layer == 0)
    {
        c->int_enabled = enabled;
    }
    c->int_disable_layer++;
}

void pop_int_disable(void)
{
    struct cpu* c = cur_cpu();
    if (--(c->int_disable_layer) < 0)
    {
        panic("popcli");
    }
    if (c->int_disable_layer == 0 && c->int_enabled)
    {
        enable_interrupt();
    }
}
