//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#ifndef GUIDE_OS_BOOT_ALLOC_H
#define GUIDE_OS_BOOT_ALLOC_H

#include "types.h"

struct boot_alloc_zone {
  void* begin_kva;
  void* end_kva;
};

void boot_alloc_init(void* start_kva, size_t size);
/**
 * Initial Allocation. Only support page sized allocations.
 * @param size n bytes, rounded up to nearest page size automatically
 * 
 * @return null or ptr to the allocated region
 */
void* boot_alloc(size_t size);

void* boot_alloc_disable(void);

#endif //GUIDE_OS_BOOT_ALLOC_H
