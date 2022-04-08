#include "systick.h"
#include "defs.h"
#include "spinlock.h"
#include "stdatomic.h"

static uint64_t sys_tick;

uint64_t systick(void) {
  uint64_t time = atomic_load_explicit(&sys_tick, memory_order_acquire);
  return time;
}

void systick_init(void) {
  atomic_store_explicit(&sys_tick, 0, memory_order_release);
}

void systick_increment(void) {
  atomic_fetch_add_explicit(&sys_tick, 1, memory_order_acq_rel);
}

void spin_sleep(uint64_t ticks) {
    uint64_t target = ticks + systick();
    while(systick() < target)
        ;
}
