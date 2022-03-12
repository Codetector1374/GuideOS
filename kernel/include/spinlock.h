//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#ifndef GUIDE_OS_SPINLOCK_H
#define GUIDE_OS_SPINLOCK_H

#include <stdatomic.h>

#include "types.h"


typedef struct spinlock {
  u8    locked;

  const char*   name;
  uptr          locking_pc;
} spinlock_t;

void init_lock(spinlock_t *lk, const char* name);
void acquire(spinlock_t *lk);
void release(spinlock_t *lk);

#endif //GUIDE_OS_SPINLOCK_H
