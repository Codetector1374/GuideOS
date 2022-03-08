//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#include "arch/x86.h"
#include "defs.h"

void disable_interrupt()
{
    cli();
}

void wait_for_interrupt() {
    hlt();
}
