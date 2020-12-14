#include <stdint.h>
#include "pshell.h"
#include "string.h"
#include "terminal.h"

char* builtin_funcs_str[] = {
    "help",
    "shrek",
    "clear",
    "echo"
};

int32_t psh_builtin_num = (sizeof(builtin_funcs_str)/sizeof(char *));

int32_t 
psh_help(char *args)
{
    term_print("$11available $11functions:\n");
    for(int i = 0; i < psh_builtin_num; ++i) 
    {
        term_print(builtin_funcs_str[i]);
        term_print("\n");
    }
    return 0;
}

int32_t echo(char *args)
{
    term_print(args+1);
    term_print("\n");
    return 0;
}

int32_t clear(char *args)
{
    term_init();
    return 0;
}

int32_t
shrek(char *args)
{
    if(args == NULL)
    {
        term_print("$12Not $12enough $12arguments!\n");
        return 1;
    }
    if(strcmp(args, " love"))
    {
        term_print("$12wrong $12argument: shrek is love, shrek is live!\n");
        return 1;
    }
    term_print("$10TRUE\n");
    return 0;
}

int32_t (*builtin_funcs[])(char *) = {
    &psh_help,
    &shrek,
    &clear,
    &echo
};


int32_t psh_exec(char *args)
{
    if(args == NULL) return 1;

    for(int32_t i = 0; i < psh_builtin_num; ++i)
    {
        if(strcmp(args, builtin_funcs_str[i]) == 0)
            return (*builtin_funcs[i])(strwhitespace(args));
    }
    term_print("$12This $12function $12dont $12exsist!\nType \'help\'\n");
    return 1;
}

void 
psh_loop(void)
{
    term_print("primitve shell: $10OK!\n");
    int32_t status = 0;
    char *buffer;

    do {
        term_print("[");
        term_print_int32(status);
        term_print("] > ");
        buffer = preadline();
        status = psh_exec(buffer);
    } while(1);
}