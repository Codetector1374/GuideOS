//
// Created by user on 2/14/2022.
//

extern void cputs(const char* string);

__attribute((noreturn))
void c_entry()
{
    cputs("We are now in C code\r\n");
    for(;;) {
    }
}