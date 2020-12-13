#include <stdint.h>
#include "pshell.h"
#include "terminal.h"

void psh_loop(void)
{
    int32_t status = 1;
    char *buffer;

    do {
        term_print("[");
        term_print_int32(status);
        term_print("] > ");
        buffer = preadline();

    } while(status);
}