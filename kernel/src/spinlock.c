//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#include "defs.h"
#include "spinlock.h"

void init_lock(spinlock_t *lk, const char* name)
{
    atomic_init(&lk->locked, 0);
    lk->cpu = NULL;
    lk->name = name;
}

bool holding(spinlock_t *lk) {
    bool r;
    push_int_disable();
    r = lk->locked && lk->cpu == cur_cpu();
    pop_int_disable();
    return r;
}

void acquire(spinlock_t *lk)
{   
    push_int_disable();
    if (holding(lk)) {
        panic("acquire");
    }
    // Use c11 atomics to acquire the lock
    //  Here we atomically exchange locked with 1.  If locked was 0, then we've
    //    just acquired the lock!
    //  We use the acquire release semantics (orderings).  We really only want
    //    acquire semantics, but we are doing a read and modify operation at once
    //    which requires acquire (write) and release (read) ordering semantics.
    while(atomic_exchange_explicit(&lk->locked, 1, memory_order_acq_rel) != 0)
        ;
    lk->cpu = cur_cpu();
}

void release(spinlock_t *lk)
{
    if (!holding(lk)) {
        panic("release");
    }
    lk->cpu = NULL;
    atomic_store_explicit(&lk->locked, 0, memory_order_release);
    pop_int_disable();
}

