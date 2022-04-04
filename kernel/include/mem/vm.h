//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//
#pragma once

/**
 * @brief setup initial page tables
 */
void vmm_init(void);
void vmm_load_ktable(void);
void* vmm_get_kernel_pgtable(void);