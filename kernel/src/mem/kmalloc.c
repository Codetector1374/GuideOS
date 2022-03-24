#include "types.h"
#include "defs.h"
#include "spinlock.h"
#include "mem/page_alloc.h"

#define TOTAL_METADATA_SIZE (sizeof(metadata_t))
#define MIN_BLOCK_SIZE      (TOTAL_METADATA_SIZE + 8)

typedef struct metadata {
    struct metadata *next;
    size_t size; // size of the user usable part of the block in number of bytes
} metadata_t;

spinlock_t kmalloc_lock;
metadata_t *size_list; // Free List

static metadata_t *merge_with_front(metadata_t *front, metadata_t *front_parent, size_t empty_Size) {
    if (front_parent == NULL) {
        size_list = front->next;
    } else {
        front_parent->next = front->next;
    }
    front->size += empty_Size;
    return front;
}

static metadata_t *merge_with_back(metadata_t *back, metadata_t *back_parent, metadata_t *original) {
    if (back_parent == NULL) {
        size_list = back->next;
    } else {
        back_parent->next = back->next;
    }
    original->size += back->size + TOTAL_METADATA_SIZE;
    return original;
}

static metadata_t *add_block(void *block, size_t raw_size) {
    if (raw_size < MIN_BLOCK_SIZE) {
        return NULL;
    }
    metadata_t *metaBlock = (metadata_t *) block;
    metaBlock->size = raw_size - TOTAL_METADATA_SIZE;
    void *endAddr = (uint8_t *) metaBlock + TOTAL_METADATA_SIZE + metaBlock->size;

    metadata_t *currentBlock = size_list;
    metadata_t *parentBlock = NULL;
    while (currentBlock) {
        if ((uint8_t *) currentBlock + currentBlock->size + TOTAL_METADATA_SIZE == block) {
            metaBlock = merge_with_front(currentBlock, parentBlock, metaBlock->size+TOTAL_METADATA_SIZE);
        } else if (endAddr == (uint8_t *) currentBlock) {
            metaBlock = merge_with_back(currentBlock, parentBlock, metaBlock);
        } else {
            parentBlock = currentBlock;
        }
        currentBlock = currentBlock->next;
    }
    if (size_list && size_list->size < metaBlock->size) {
        metadata_t *loopBlock = size_list;
        while (loopBlock && loopBlock->next) {
            if (loopBlock->next->size >= metaBlock->size) {
                metaBlock->next = loopBlock->next;
                loopBlock->next = metaBlock;
                return metaBlock;
            }
            loopBlock = loopBlock->next;
        }
        if (loopBlock) {
            metaBlock->next = loopBlock->next;
            loopBlock->next = metaBlock;
        }
    } else {
        metaBlock->next = size_list;
        size_list = metaBlock;
    }
    return metaBlock;
}

static void *remove_block(metadata_t *block, metadata_t *parent, size_t request_size) {
    if (parent) {
        parent->next = block->next;
    } else {
        size_list = block->next;
    }
    if (block->size == request_size) {
        return (block + 1);
    } else {
        size_t oldBlockSize = block->size - (request_size + TOTAL_METADATA_SIZE);
        add_block((void *) block, oldBlockSize + TOTAL_METADATA_SIZE);
        metadata_t *newBlock = (metadata_t *) (((uint8_t *) block) + TOTAL_METADATA_SIZE + oldBlockSize);
        newBlock->size = request_size;
        return (newBlock + 1);
    }
}

static void *find_fit_block(size_t request_size) {
    metadata_t *current_block = size_list;
    metadata_t *parent = NULL;
    while (current_block) {
        if (current_block->size == request_size || current_block->size >= request_size + MIN_BLOCK_SIZE) {
            return remove_block(current_block, parent, request_size);
        }
        parent = current_block;
        current_block = current_block->next;
    }
    return NULL;
}

void kmalloc_init(void)
{
  size_list = NULL;
  init_lock(&kmalloc_lock, "kmalloc");
}

/* MALLOC
 * See PDF for documentation
 */
void *kmalloc(size_t size) {
  void *rtn = NULL;
  acquire(&kmalloc_lock);

  if (size == 0) {
    goto kmalloc_exit;
  }
  if (size > (PG_SIZE - TOTAL_METADATA_SIZE)) {
    goto kmalloc_exit;
  }
  rtn = find_fit_block(size);
  if (rtn) 
  {
    goto kmalloc_exit;
  } 
  else
  {
    void *newRam = pgalloc_allocate(0); // 0th order allocates 1 page
    if (newRam)
    {
      add_block(newRam, PG_SIZE);
      rtn = find_fit_block(size);
    }
    else 
    {
      panic("malloc: OOM");
    }
    goto kmalloc_exit;
  }

kmalloc_exit:
  release(&kmalloc_lock);
  return rtn;
}

void *krealloc(void *ptr, size_t size) {
    if (ptr == NULL) {
        return kmalloc(size);
    }
    if (size == 0) {
        kfree(ptr);
        return NULL;
    }
    metadata_t *block = ((metadata_t *) ptr) - 1;
    void* newBlk = kmalloc(size);
    memcpy(newBlk, ptr, MIN(size, block->size));
    kfree(ptr);
    return newBlk;
}

/* CALLOC
 * See PDF for documentation
 */
void *kcalloc(size_t nmemb, size_t size) {
    void *memory = kmalloc(nmemb * size);
    if (memory) {
        memset(memory, 0, size * nmemb);
    }
    return memory;
}

void kfree(void *ptr) {
    if (ptr == NULL) {
        return;
    }
    metadata_t *block = ((metadata_t *) ptr) - 1;
    acquire(&kmalloc_lock);
    add_block((void *) block, block->size + TOTAL_METADATA_SIZE);
    release(&kmalloc_lock);
}

