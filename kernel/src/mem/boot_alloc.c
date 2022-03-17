//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#include "defs.h"
#include "mem/boot_alloc.h"

static struct boot_alloc_zone default_zone = {
        (void*) 0,
        (void*) 0
};

static bool boot_alloc_initialized = FALSE;

/**
 * Initialize the boot allocator with the given start kva and size
 * @param start_kva
 * @param size
 */
void boot_alloc_init(void *start_kva, size_t size) {
    if (boot_alloc_initialized) {
        return;
    }
    boot_alloc_initialized = TRUE;

    // Force native size alignment
    default_zone.begin_kva = (void*)ROUNDUP_PWR2((size_t)start_kva, PG_SIZE);
    default_zone.end_kva = (void*)((size_t)start_kva + size);
}

void *boot_alloc(size_t size) {
    size = ROUNDUP_PWR2(size, PG_SIZE);

    if (default_zone.begin_kva + size < default_zone.end_kva) {
        void* return_ptr = default_zone.begin_kva;
        default_zone.begin_kva += size;
        return return_ptr;
    }

    return NULL;
}
