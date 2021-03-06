#ifndef GUIDE_OS_CONSOLE_H
#define GUIDE_OS_CONSOLE_H

void console_init(void);
void putchar(int c);
/**
 * @brief Blocking getc from console (Will sleep)
 * 
 * @return int character
 */
int console_getc(void);
void console_isr_handler(void);

#endif //GUIDE_OS_CONSOLE_H
