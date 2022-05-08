#include "types.h"
#include "defs.h"
#include "arch/x86/instructions.h"
#include "usys.h"
#include "proc.h"

#define CMD_MAX_LEN 1024
#define CMD_MAX_PARTS 32

void ksh_read_line(char* buf) {
  size_t input_len = 0;
  for(;;) {
    int c;
    c = console_getc();
    if (c < 0) {
      panic("getc");
    }
    if (c == BACKSPACE && input_len > 0) {
      input_len--;
      putchar(BACKSPACE);
    } else if (c == '\r' || c == '\n') {
      putchar('\n');
      buf[input_len] = '\0';
      break;
    } else if (input_len < CMD_MAX_LEN && c > 0 && c < 0xFF) {
      buf[input_len++] = (char)c;
      putchar(c);
    }
  }
}

int ksh_split_line(char* line, char** tokens) {
  int token_position = 0;
  int line_position = 0;
  if (line == NULL || tokens == NULL) {
    return 0;
  }

  while(line[line_position] != '\0' && line_position < CMD_MAX_LEN && token_position < CMD_MAX_LEN) {
    tokens[token_position++] = &line[line_position];
    while(line[line_position] != ' ' && line[line_position] != '\0') {
      line_position++;
    }
    while(line[line_position] == ' ' && line[line_position] != '\0') {
      line[line_position++] = '\0';
    }
  }
  tokens[token_position] = NULL;
  return token_position;
}

int echo(int argc, char** argv) {
  for (size_t i = 1; i < argc; i++) {
    kprintf("%s ", argv[i]);
  }
  kprintf("\n");
  return 0;
}

struct sh_builtin {
  const char* cmd;
  int (*fn)(int argc, char** argv);
};

static struct sh_builtin built_ins[] = {
  {"echo", echo},
};

void kshell_main(void) {
  char buffer[CMD_MAX_LEN];
  char* parts[CMD_MAX_PARTS];
  for(;;) {
    kprintf("$> ");
    ksh_read_line(buffer);
    int argc = ksh_split_line(buffer, parts);
    int (*fn)(int argc, char** argv) = NULL;
    if (argc == 0) {
      kprintf("\n");
      continue;
    }
    for (size_t i = 0; i < sizeof(built_ins) / sizeof(built_ins[0]); i++) {
      if (strcmp(built_ins[i].cmd, parts[0]) == 0) {
        fn = built_ins[i].fn;
        break;
      }
    }
    if (fn != NULL) {
      fn(argc, parts);
    } else {
      kprintf("Command not found: %s\n", parts[0]);
    }
  }
  for(;;){}
}
