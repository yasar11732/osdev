
#include <stdio.h>

#ifdef __is_libk
#include <kernel/tty.h>
#endif

int putchar(int ic) {

    #ifdef __is_libk
        terminal_putchar(ic);
    #else
        // Todo: Make Userspace putchar
    #endif

    return ic;
}