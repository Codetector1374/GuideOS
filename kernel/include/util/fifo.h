#pragma once

#include "types.h"
#include <stdatomic.h>

typedef struct spsc_fifo {
  u8* buffer;
  size_t buffer_size;
  atomic_size_t read;
  atomic_size_t watermark;
  atomic_size_t write;
} spsc_fifo_t;

os_error_t spsc_fifo_init(spsc_fifo_t* f, u8* buffer, size_t buffer_size);
os_error_t spsc_fifo_write(spsc_fifo_t* f, u8 byte);
int spsc_fifo_read(spsc_fifo_t* f, u8* pOut);