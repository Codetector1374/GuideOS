#pragma once

#include "types.h"

uint64_t syscall0(uint64_t);
uint64_t syscall1(uint64_t, uint64_t);
// uint64_t syscall2(uint64_t, uint64_t, uint64_t);
// uint64_t syscall3(uint64_t, uint64_t, uint64_t, uint64_t);


void pause(void);
