#pragma once

#include "types.h"

void pgalloc_init(size_t max_mem_addr);
/**
 * @brief free the selected range. Can be used to feed the allocator initially
 * 
 * @param start_va pg aligned start address
 * @param end_va pg aligned end address
 */
void pgalloc_free_range(void *start_va, void *end_va);

void* pgalloc_allocate(int order);
void pgalloc_free(void* addr, int order);