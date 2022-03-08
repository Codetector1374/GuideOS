//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#ifndef GUIDE_OS_DEFS_H
#define GUIDE_OS_DEFS_H

#include "types.h"
#include "kprintf.h"
#include "vm.h"
#include "string.h"

#define ROUNDUP_PWR2(x, sz)      (((x) + ((sz) - 1)) & ((sz) - 1))
#define ROUNDDOWN_PWR2(x, sz)    ((x) & ((sz) - 1))

void panic(char *s);
void disable_interrupt(void);
void wait_for_interrupt(void);

#endif //GUIDE_OS_DEFS_H
