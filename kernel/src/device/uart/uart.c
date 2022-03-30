//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//
#include "device/uart.h"
#include "arch/x86/instructions.h"
#include "arch/x86/interrupt.h"
#include "defs.h"
#include "types.h"

static void uart_write(uart_device_t* u, u8 offset, u8 value) {
  if (u->mmio_base) {
    ((volatile u32*)u->mmio_base)[offset] = value;
    (void)((volatile u32*)u->mmio_base)[offset];
  } else {
    outb(u->pio_base + offset, value);
  }
}

static u8 uart_read(uart_device_t* u, u8 offset) {
  if (u->mmio_base) {
    return (u8)((volatile u32*)u->mmio_base)[offset];
  } else {
    return inb(u->pio_base + offset);
  }
}

static int uart_init(uart_device_t* u) {
  uart_write(u, 3, 0x80);            // Unlock divisor
  uart_write(u, 0, 115200 / 115200); // Baud = 115200
  uart_write(u, 1, 0);               // Upper divisor (part of baud 115200)
  uart_write(u, 3, 0x3);             // Lock divisor + 8 N 1
  uart_write(u, 1, 0);               // Disable Interrupt
  uart_write(u, 2, 0x7);             // Enable and reset TX RX FIFO

  uart_write(u, 7, 0x69);
  if (uart_read(u, 7) != 0x69) {
    return -1;
  }

  uart_write(u, 1, 0x1); // Enable Rx and Tx Interrupt
  
  {
    const char* init_string = "..uart..\n";
    int i = 0;
    while(init_string[i] != '\0')
      uart_putc(u, init_string[i++]);
  }
  return 0;
}

int uart_pio_init(uart_device_t* u, u16 pio_base) {
  u->mmio_base = NULL;
  u->pio_base = pio_base;
  return uart_init(u);
}

int uart_mmio_init(uart_device_t* u, void* mmio_base) { return 1; }

void s_uart_putc(uart_device_t *u, int c) {
  while ((uart_read(u, 5) & 0x20) == 0)
    ;
  uart_write(u, 0, c);
}

void uart_putc(uart_device_t* u, int c) {
  if (!u)
    return;
  if (c == BACKSPACE) {
    s_uart_putc(u, '\x08');
    s_uart_putc(u, ' ');
    s_uart_putc(u, '\x08');
  } else {
    s_uart_putc(u, c);
  }
}

bool uart_hasbyte(uart_device_t* u) {
  if (!u)
    return FALSE;

  return (uart_read(u, 5) & 0x1) != 0;
}

int uart_getc(uart_device_t* u) {
  if (!uart_hasbyte(u))
    return -1;
  return uart_read(u, 0);
}
