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
void* boot_alloc(size_t size);

#endif //GUIDE_OS_BOOT_ALLOC_H
