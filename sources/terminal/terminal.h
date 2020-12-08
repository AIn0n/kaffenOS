#ifndef _TERM_H_
#define _TERM_H_

void term_init();
void term_print(const char* str);
void term_print_int32(int32_t a);
void term_print_uint32(uint32_t a, uint8_t base);
void term_putc(char c);
void preadline(void);
#endif