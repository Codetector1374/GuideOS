//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//
#include "defs.h"
#include "mem/boot_alloc.h"
#include "mem/vm.h"
#include "mem/memlayout.h"
#include "arch/x86/vmm.h"
#include "arch/x86/instructions.h"

// Kernel PML4
static pml4e_t *kern_pml4;

void vmm_init(void) {
  kern_pml4 = boot_alloc(PG_SIZE);
  if (!kern_pml4) panic("vmm_init oom");
  if ((size_t)kern_pml4 % PG_SIZE != 0) panic("align");
  memset(kern_pml4, 0, PG_SIZE);

  pdpe_t *kern_phys_map_pdp = 0;
  for (size_t va = KERNBASE; va < KERN_PHYMAP_TOP; va += L3_HUGE_SIZE) {
    if (PG_L3_IDX(va) == 0) {
      kern_phys_map_pdp = boot_alloc(PG_SIZE);
      if (!kern_phys_map_pdp) panic("vmm_init oom(l3)");
      if ((size_t)kern_phys_map_pdp % PG_SIZE != 0) panic("align");

      memset(kern_phys_map_pdp, 0, PG_SIZE);

      kern_pml4[PG_L4_IDX(va)] = PTE_FLAG_P | PTE_FLAG_WRITE | PTE_ADDR(KV2P(kern_phys_map_pdp));
    }
    if (!kern_phys_map_pdp) panic("vmm_init nullptr");
    kern_phys_map_pdp[PG_L3_IDX(va)] = PTE_FLAG_P | PTE_FLAG_WRITE | PTE_FLAG_SZ | PTE_ADDR(KV2P(va));
  }
  vmm_load_ktable();
}

void vmm_load_ktable()
{
  lcr3(KV2P(kern_pml4));
}