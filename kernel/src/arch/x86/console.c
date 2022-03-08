#include "console.h"
#include "types.h"
#include "vm.h"
#include "arch/x86.h"
#include "string.h"
#include "defs.h"

#define NUM_COL     80
#define NUM_ROW     25

#define BACKSPACE 0x100
#define CRTPORT 0x3d4

static u16* crt = (u16*) P2KV(0xb8000);


void console_init() {
}

void putchar(int c) {
    int pos;

    // Cursor position: col + 80*row.
    outb(CRTPORT, 14);
    pos = inb(CRTPORT+1) << 8;
    outb(CRTPORT, 15);
    pos |= inb(CRTPORT+1);

    if(c == '\n')
        pos += 80 - pos%80;
    else if(c == BACKSPACE){
        if(pos > 0) --pos;
    } else {
        crt[pos++] = (c&0xff) | 0x0700;  // black on white
    }

    if(pos < 0 || pos > 25*80) {
//        panic("pos under/overflow");
    }

    if((pos/80) >= 24){  // Scroll up.
        memcpy(crt, crt+80, sizeof(crt[0])*23*80);
        pos -= 80;
        memset(crt+pos, 0, sizeof(crt[0])*(24*80 - pos));
    }

    outb(CRTPORT, 14);
    outb(CRTPORT+1, pos>>8);
    outb(CRTPORT, 15);
    outb(CRTPORT+1, pos);
    crt[pos] = ' ' | 0x0700;
}
