#include "types.h"
#include "util/fifo.h"

os_error_t spsc_fifo_init(spsc_fifo_t* f, char* buffer, size_t buffer_size) {
  if (!f) return OSERR_INVALID_ARGUMENT;
  f->buffer = buffer;
  f->buffer_size = buffer_size;
  atomic_init(&f->read, 0);
  atomic_init(&f->write, 0);
  return OSERR_OK;
}

os_error_t spsc_fifo_write(spsc_fifo_t* f, char byte) {
  if (!f) return OSERR_INVALID_ARGUMENT;
  size_t cur_read = atomic_load_explicit(&f->read, memory_order_acquire);
  size_t cur_write = atomic_load_explicit(&f->write, memory_order_acquire);
  if ((cur_write + 1) % f->buffer_size == cur_read) {
    return OSERR_NO_MEMORY;
  }
  f->buffer[cur_write] = byte;
  atomic_store_explicit(&f->write, ((cur_write + 1) % f->buffer_size), memory_order_release);
  return OSERR_OK;
}

int spsc_fifo_read(spsc_fifo_t* f) {
  if (!f) return -1;
  size_t cur_read = atomic_load_explicit(&f->read, memory_order_acquire);
  size_t cur_write = atomic_load_explicit(&f->write, memory_order_acquire);
  if (cur_read == cur_write) return -1;
  int rtn = f->buffer[cur_read];
  atomic_store_explicit(&f->read, (cur_read + 1) % f->buffer_size, memory_order_release);
  return rtn;
}
