//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#include "defs.h"

__attribute__((noreturn))
void
panic(char *s)
{
    disable_interrupt();
//    cons.locking = 0;
//    kprintf("cpu with apicid %d: panic: ", lapicid());
    kprintf("=====PANIC====\n");
    kprintf(s);
    kprintf("\n");
    for(;;) {
        wait_for_interrupt();
    }
}