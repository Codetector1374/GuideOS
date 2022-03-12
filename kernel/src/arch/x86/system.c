//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#include "arch/x86.h"
#include "defs.h"

bool disable_interrupt()
{
    size_t flags = read_flags();
    cli();
    return (flags & FL_IF) > 0;
}

void enable_interrupt() {
    sti();
}

void wait_for_interrupt() {
    hlt();
}
