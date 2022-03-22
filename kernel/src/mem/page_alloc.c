//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#include "types.h"
#include "defs.h"
#include "mem/memlayout.h"
#include "mem/boot_alloc.h"
#include "spinlock.h"

#define MAX_ORDER 10

#define BITMAP_TOGGLE(bitmap, addr, order)

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
static spinlock_t pgalloc_lock;
static size_t max_addr = 0;

/**
 * Initialize the pgalloc page allocator
 * @param max_mem_addr the highest kva where memory still exists
 */
void pgalloc_init(size_t max_mem_addr)
{
  if (max_addr != 0)
  {
      panic("pgalloc_init double init");
  }
  init_lock(&pgalloc_lock, "pgalloc");
  max_addr = PGROUNDUP((size_t)max_mem_addr);
  memset(pgalloc_bins, 0, sizeof(pgalloc_bins));

  size_t num_pages = max_addr / PG_SIZE;
  size_t bitmap_size = num_pages / 8;
  for (int i = 0; i <= MAX_ORDER; ++i) 
  {
    // For each bucket, we only need number of chunk / 2 bits.
    bitmap_size = (bitmap_size + 1) / 2;
    if ((pgalloc_bins[i].bitmap = boot_alloc(bitmap_size)) == NULL)
    {
      panic("pgalloc_init");
    }
  }
}

/**
 * @brief free_block helper. Mark a block of memory 
 * free and add it to the free list. 
 * @note pgalloc_lock must be held before calling this function
 * 
 * @param pgidx index of the PHYSICAL page to free
 * @param order 2^order pages to be freed
 */
static void s_free_block(size_t pgidx, int order) 
{
  if ((pgidx & ((1UL << order) - 1)) != 0)
  {
    kprintf("free alignment pgidx %lx, order %d\n", pgidx, order);
    panic("s_free_block");
  }
  size_t bit_index = pgidx >> (order + 1);
  struct free_page *pg_va = (struct free_page*)(P2KV(pgidx * PG_SIZE));
  struct page_alloc_free_bin* bin = &pgalloc_bins[order];
  bool hasBuddy = (bin->bitmap[bit_index / (sizeof(u32) * 8)] & (1U << (bit_index % (sizeof(u32) * 8)))) != 0;
  if (order == MAX_ORDER) hasBuddy = 0;
  bin->bitmap[bit_index / (sizeof(u32) * 8)] ^= 1U << (bit_index % (sizeof(u32) * 8));
  if (hasBuddy)
  {
    void* buddy_va = P2KV((pgidx ^ (1UL << order)) * PG_SIZE);
    struct free_page** pCurrent = &(bin->list.head);
    while(*pCurrent != NULL && *pCurrent != buddy_va) {
      pCurrent = &((*pCurrent)->next);
    }
    if (*pCurrent == buddy_va) {
      *pCurrent = (*pCurrent)->next;
      bin->list.size--;
      s_free_block(pgidx & ~((1UL << (order+1))-1), order+1);
    } else {
      kprintf("looking for buddy %p, self = %p\n", buddy_va, pg_va);
      struct free_page *pFree = bin->list.head;
      while(pFree != NULL) {
        kprintf("%p ->\n", pFree);
        pFree = pFree->next;
      }
      panic("has buddy but can't find it???");
    }
  } else {
    pg_va->next = bin->list.head;
    bin->list.head = pg_va;
    bin->list.size++;
  }
}

static void* s_alloc_block(int order)
{
  if (order < 0 || order > MAX_ORDER)
  {
    return NULL;
  }
  struct page_alloc_free_bin* bin = &pgalloc_bins[order];
  if (bin->list.size)
  {
    bin->list.size--;
    struct free_page *pg = bin->list.head;
    bin->list.head = pg->next;
    size_t pgidx = PG_INDEX(KV2P(pg));
    size_t bit_index = pgidx >> (order + 1);
    bin->bitmap[bit_index / (sizeof(u32) * 8)] ^= 1U << (bit_index % (sizeof(u32) * 8));
    return (void*)pg;
  }
  else
  {
    void *large_page = s_alloc_block(order + 1);
    if (!large_page)
    {
      return NULL;
    }
    s_free_block(PG_INDEX(KV2P(large_page + (PG_SIZE << order))), order);
    return (void*) large_page;
  }
}

void pgalloc_free_range(void *start_va, void *end_va) 
{
  size_t start_pgidx = PGROUNDUP((size_t) KV2P(start_va)) / PG_SIZE;
  size_t end_pgidx = PGROUNDDOWN((size_t) KV2P(end_va)) / PG_SIZE;

  acquire(&pgalloc_lock);
  while(start_pgidx < end_pgidx)
  {
    for (int pg_order = MAX_ORDER; pg_order >= 0; pg_order--)
    {
      if (((start_pgidx & ((1UL << pg_order) - 1)) == 0) &&
          (start_pgidx + (1UL << pg_order) <= end_pgidx))
      {
        s_free_block(start_pgidx, pg_order);
        start_pgidx += (1UL << pg_order);
        break;
      }
    }
  }
  release(&pgalloc_lock);
}

void* pgalloc_allocate(int order)
{
  void* rtn;
  acquire(&pgalloc_lock);
  rtn = s_alloc_block(order);
  release(&pgalloc_lock);
  return rtn;
}

void pgalloc_free(void* addr, int order)
{
  acquire(&pgalloc_lock);
  s_free_block(PG_INDEX(KV2P(addr)), order);
  release(&pgalloc_lock);
}
