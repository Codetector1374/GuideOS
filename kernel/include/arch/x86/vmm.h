//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#pragma once
#include "types.h"

#define PTE_ADDR(pte)     ((pte) & 0xFFFFFFFFFFFFF000)
#define PTE_FLAG(pte)     ((pte) & 0x0000000000000FFF)
#define   PTE_FLAG_P      (1UL << 0U)
#define   PTE_FLAG_WRITE  (1UL << 1U)
#define   PTE_FLAG_USER   (1UL << 2U)
#define   PTE_FLAG_PWT    (1UL << 3U)
#define   PTE_FLAG_PCD    (1UL << 4U)
#define   PTE_FLAG_ACCESS (1UL << 5U)
#define   PTE_FLAG_DIRTY  (1UL << 6U)
#define   PTE_FLAG_SZ     (1UL << 7U)
#define   PTE_FLAG_PT_PAT (1UL << 7U) // Applies only to PTE
#define   PTE_FLAG_GLOBAL (1UL << 8U)
#define   PTE_FLAG_PAT    (1UL << 12U) // Applies only to PTE


#define PG_OFFSET(addr) ((addr) & 0xFFF)
#define PG_L1_IDX(addr) ((addr >> 12) & 0x1FF)
#define PG_L2_IDX(addr) ((addr >> (12 + 9)) & 0x1FF)
#define PG_L3_IDX(addr) ((addr >> (12 + 18)) & 0x1FF)
#define PG_L4_IDX(addr) ((addr >> (12 + 27)) & 0x1FF)


typedef uint64_t pte_t;
typedef uint64_t pde_t;
typedef uint64_t pdpe_t;
typedef uint64_t pml4e_t;

