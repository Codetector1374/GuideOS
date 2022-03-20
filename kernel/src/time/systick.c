#include "systick.h"
#include "defs.h"
#include "spinlock.h"

static uint64_t sys_tick;
static spinlock_t sys_tick_lock;

uint64_t systick(void)
{
    uint64_t time;
    acquire(&sys_tick_lock);
    time = sys_tick;
    release(&sys_tick_lock);
    return time;
}

void systick_init(void)
{
    init_lock(&sys_tick_lock, "sys_tick");
    sys_tick = 0;
}

void systick_increment(void)
{
    acquire(&sys_tick_lock);
    sys_tick++;
    release(&sys_tick_lock);
}

