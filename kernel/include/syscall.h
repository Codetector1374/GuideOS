#pragma once

#include "types.h"
#include "arch/x86/interrupt.h"


void handle_syscall(trapframe_t *);
