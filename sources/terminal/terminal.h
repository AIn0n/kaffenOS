#ifndef _TERM_H_
#define _TERM_H_

#define VGA_BLACK       (uint8_t)0
#define VGA_BLUE        (uint8_t)1
#define VGA_GREEN       (uint8_t)2
#define VGA_CYAN        (uint8_t)3
#define VGA_RED         (uint8_t)4
#define VGA_MAGENTA     (uint8_t)5
#define VGA_BROWN       (uint8_t)6
#define VGA_LGREY       (uint8_t)7
#define VGA_DGREY       (uint8_t)8
#define VGA_LBLUE       (uint8_t)9
#define VGA_LGREEN      (uint8_t)10
#define VGA_LCYAN       (uint8_t)11
#define VGA_LRED        (uint8_t)12
#define VGA_LMAGENTA    (uint8_t)13
#define VGA_LBROWN      (uint8_t)14
#define VGA_WHITE       (uint8_t)15
void term_init();
void term_print(const char* str);
void term_print_int32(int32_t a);
void term_print_uint32(uint32_t a, uint8_t base);
void term_putc(char c, uint8_t fg);
char* preadline(void);
void preadline_flush(void);
#endif