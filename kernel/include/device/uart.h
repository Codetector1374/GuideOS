//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#ifndef GUIDE_OS_UART_H
#define GUIDE_OS_UART_H

#include "types.h"
#include "spinlock.h"

#define UART_COM1     0x3F8

typedef struct uart_device {
  void *mmio_base;
  u16   pio_base;
} uart_device_t;

/**
 * @brief Construct a uart device with pio address pio_base
 * 
 * @param u the struct to operate on
 * @param pio_base 
 * @return 0 on success
 */
int uart_pio_init(uart_device_t *u, u16 pio_base);
int uart_mmio_init(uart_device_t *u, void* mmio_base);
void uart_putc(uart_device_t *u, int c);

#endif //GUIDE_OS_UART_H
