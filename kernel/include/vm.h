//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#ifndef GUIDE_OS_VM_H
#define GUIDE_OS_VM_H

#include "types.h"

#define KBASE       (0xFFFF800000000000)

#define KV2P(VA)    (((size_t)VA) - KBASE)
#define P2KV(PA)    ((void*)(((size_t)PA) + KBASE))

#endif //GUIDE_OS_VM_H
