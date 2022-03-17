//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#pragma once
#include "types.h"

#define PTE_ADDR(pte)     ((pte) & 0xFFFFFFFFFFFFF000)
#define PTE_FLAG(pte)     ((pte) & 0x0000000000000FFF)
#define   PTE_FLAG_P      (1U << 0U)
#define   PTE_FLAG_WRITE  (1U << 0U)
#define   PTE_FLAG_USER   (1U << 0U)
#define   PTE_FLAG_PWT    (1U << 0U)
#define   PTE_FLAG_PCD    (1U << 0U)
#define   PTE_FLAG_ACCESS (1U << 0U)
#define   PTE_FLAG_SZ     (1U << 0U)
#define   PTE_FLAG_GLOBAL (1U << 0U)

typedef uint64_t pte_t;
typedef uint64_t pde_t;
typedef uint64_t pdpe_t;
typedef uint64_t pml4e_t;
