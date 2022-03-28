#include "console.h"
#include "arch/x86.h"
#include "arch/x86/interrupt.h"
#include "defs.h"
#include "device/uart.h"
#include "mem/memlayout.h"
#include "spinlock.h"
#include "string.h"
#include "util/fifo.h"
#include "types.h"

#define NUM_COL 80
#define NUM_ROW 25

#define BACKSPACE 0x100
#define CRTPORT 0x3d4

static u16* crt = (u16*)P2KV(0xb8000);

static spinlock_t console_lock;
static uart_device_t con_uart;
static bool uart_enable;

//
// console fifo is protected by two locks:
// console_lock for reading operation and 
// console_fifo_write_lock for write operation.
//
static spsc_fifo_t console_fifo;
static spinlock_t console_fifo_write_lock;

void console_init() {
  init_lock(&console_lock, "console");
  init_lock(&console_fifo_write_lock, "console fifo");
  char *con_buffer = kmalloc(256); // 256 byte buffer is quite large for console
  if (!con_buffer) panic("console init oom");
  spsc_fifo_init(&console_fifo, con_buffer, 256);
  if (!uart_pio_init(&con_uart, UART_COM1)) {
    ioapic_unmask(4, IDT_ENTRY_IRQ_COM1, lapic_id());
    uart_enable = TRUE;
  } else {
    uart_enable = FALSE;
  }
}

void cga_putchar(int c) {
  int pos;

  // Cursor position: col + 80*row.
  outb(CRTPORT, 14);
  pos = inb(CRTPORT + 1) << 8;
  outb(CRTPORT, 15);
  pos |= inb(CRTPORT + 1);

  if (c == '\n')
    pos += 80 - pos % 80;
  else if (c == BACKSPACE) {
    if (pos > 0)
      --pos;
  } else {
    crt[pos++] = (c & 0xff) | 0x0700; // black on white
  }

  if (pos < 0 || pos > 25 * 80) {
    //        panic("pos under/overflow");
  }

  if ((pos / 80) >= 24) { // Scroll up.
    memcpy(crt, crt + 80, sizeof(crt[0]) * 23 * 80);
    pos -= 80;
    memset(crt + pos, 0, sizeof(crt[0]) * (24 * 80 - pos));
  }

  outb(CRTPORT, 14);
  outb(CRTPORT + 1, pos >> 8);
  outb(CRTPORT, 15);
  outb(CRTPORT + 1, pos);
  crt[pos] = ' ' | 0x0700;
}

void putchar(int c) {
  acquire(&console_lock);
  cga_putchar(c);
  if (uart_enable) {
    if (c == '\n') {
      uart_putc(&con_uart, '\r');
    }
    uart_putc(&con_uart, c);
  }
  release(&console_lock);
}

/**
 * @brief Console ISR handler. This function expects
 * to be called with interrupt disabled. 
 * @todo Might consider adding a lock for the UART
 */
void console_isr_handler(void) {
  if (uart_enable) {
    int uartc;
    while ((uartc = uart_getc(&con_uart)) >= 0) {
      acquire(&console_fifo_write_lock);
      if (spsc_fifo_write(&console_fifo, (char)uartc)) {
        kprintf("uart buffer full\n");
      }
      release(&console_fifo_write_lock);
    }
  }
}
