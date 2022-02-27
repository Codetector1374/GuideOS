//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#ifndef GUIDE_OS_VM_H
#define GUIDE_OS_VM_H

#include "types.h"

#define KERNBASE       (0xFFFF800000000000)

#define KV2P(VA)    (((size_t)VA) - KERNBASE)
#define P2KV(PA)    ((void*)(((size_t)PA) + KERNBASE))

// Paging related defines

#define PG_SIZE     (4096)

#define NPTENTRIES  (512)

#define PGROUNDUP(sz)  (((sz)+PGSIZE-1) & ~(PGSIZE-1))
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE-1))



extern  size_t kern_end[];

#endif //GUIDE_OS_VM_H
