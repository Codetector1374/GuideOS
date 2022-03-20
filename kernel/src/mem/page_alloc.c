//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#include "types.h"
#include "defs.h"
#include "mem/boot_alloc.h"

#define MAX_ORDER 10

struct free_page {
  struct free_page  *next;
};

struct free_page_list {
  struct free_page  *head;
  size_t            size;
};

struct page_alloc_free_bin {
  struct free_page_list     list;
  u32                       *bitmap;
};

static struct page_alloc_free_bin  pgalloc_bins[MAX_ORDER + 1];
static void* max_addr = NULL;

/**
 * Initialize the pgalloc page allocator
 * @param max_mem_addr the highest kva where memory still exists
 */
void pgalloc_init(void* max_mem_addr)
{
  if (max_addr != NULL)
  {
      panic("pgalloc_init double init");
  }
  max_addr = (void*) PGROUNDDOWN((size_t)max_mem_addr);
  memset(pgalloc_bins, 0, sizeof(pgalloc_bins));

  size_t size = (size_t) max_addr;
  size_t num_pages = size / PG_SIZE;
  size_t bitmap_size = num_pages / 8;
  for (int i = 0; i <= MAX_ORDER; ++i) 
  {
    if ((pgalloc_bins[i].bitmap = boot_alloc(bitmap_size)) == NULL)
    {
      panic("pgalloc_init");
    }
    bitmap_size = (bitmap_size + 1) / 2;
  }
}
