//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#ifndef GUIDE_OS_MEMLAYOUT_H
#define GUIDE_OS_MEMLAYOUT_H

#include "types.h"

#define KERNBASE        (0xFFFF800000000000)
#define KERN_PHYMAP_TOP (KERNBASE + (512UL << 30U)) // 512 GB

#define KV2P(VA)    (((size_t)VA) - KERNBASE)
#define P2KV(PA)    ((void*)(((size_t)PA) + KERNBASE))

// Paging related defines
#define NPTENTRIES  (512)

#define PG_SIZE       (4096)
#define L2_HUGE_SIZE  (PG_SIZE * NPTENTRIES)
#define L3_HUGE_SIZE  (L2_HUGE_SIZE * NPTENTRIES)

#define PGROUNDUP(sz)  (((sz)+PG_SIZE-1) & ~(PG_SIZE-1))
#define PGROUNDDOWN(a) (((a)) & ~(PG_SIZE-1))



extern  size_t kern_end[];

#endif //GUIDE_OS_MEMLAYOUT_H
