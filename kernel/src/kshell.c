#include "types.h"
#include "defs.h"
#include "arch/x86/instructions.h"
#include "usys.h"

#define CMD_MAX_LEN 1024

void kshell_main(void) {
  pause();
  kprintf("\n\n");
  size_t input_len = 0;
  u8 buffer[CMD_MAX_LEN];
  for(;;) {
    input_len = 0;
    kprintf("$> ");
    for(;;) {
      int c;
      wait_for_interrupt();
      while((c = console_getc()) < 0) {
        spin_sleep(1);
      }
      if (c == BACKSPACE && input_len > 0) {
        input_len--;
        putchar(BACKSPACE);
      } else if (c == '\r' || c == '\n') {
        putchar('\n');
        break;
      } else if (input_len < CMD_MAX_LEN && c > 0 && c < 0xFF) {
        buffer[input_len++] = (u8)c;
        putchar(c);
      }
    }

  }
  for(;;){}
}