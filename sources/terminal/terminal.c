#include <stdint.h>
#include "terminal.h"
#include "keyboard.h"
#include "math.h"
#include "misc.h"

//for vga display
volatile uint16_t* vga_buffer = (uint16_t *) 0xB8000;
const int VGA_COLS = 80;
const int VGA_ROWS = 25;

//for terminal
uint32_t term_col;
uint32_t term_row;

static inline uint16_t vga_entry(unsigned char uc, uint8_t fg, uint8_t bg)
{
    //VGA_CHAR BBBBFFFFCCCCCCCC, B-background F-foreground C-ASCII char
    uint16_t color = fg | bg << 4;
    return (uint16_t) color << 8 | (uint16_t) uc;
};

void term_init()
{
    term_col = 0; term_row = 0;
    for(int col = 0; col < VGA_COLS; ++col)
    {
        for(int row = 0; row < VGA_ROWS; ++row)
        {
            const uint32_t index = (VGA_COLS * row) + col;
            vga_buffer[index] = vga_entry(' ', 0, 0);          
        }
    }
}

void term_putc(char c)
{
    switch(c)
    {
    case '\n':
        {
            term_col = 0;
            ++term_row;
            break;
        }
    default:
        {
            const uint32_t index = (VGA_COLS * term_row) + term_col;
            vga_buffer[index] = vga_entry(c, 15, 0);
            ++term_col;
            break;
        }
    }
    if(term_col >= VGA_COLS){   term_col =0;    ++term_row;     }
    if(term_row >= VGA_ROWS){   term_col = 0;   term_row = 0;   }
};

void term_print(const char* str){
    for(uint32_t i = 0; str[i] != '\0'; ++i) term_putc(str[i]);
}

void term_print_int32(int32_t a)
{
    uint8_t minus = 0;
    int32_t b = a, len = 0;

    if( a < 0) {minus = 1; a = -(a);}   //checking the sign
    do { b /= 10; ++len;}while(b > 0);  //finding length
    if(minus) term_putc('-');           //putting "-" if a < 0
    for(int32_t i = len; i > 0; --i)    //putting every char in for loop
    {
        b = a;
        b /= (int)pow_rec(10, i - 1);
        b %= 10;
        term_putc(b + '0');
    }
}

void term_print_uint32(uint32_t a, uint8_t base)
{
    int32_t b = a, len = 0;
    do { b /= base; ++len;}while(b > 0);  //finding length
    for(int32_t i = len; i > 0; --i)    //putting every char in for loop
    {
        b = a;
        b /= (int)pow_rec(base, i - 1);
        b %= base;
        term_putc(b + ((b>9) ? '7' : '0'));
    }
}

//--------------primitive readline-------

#define PREADLINE_BUFF_SIZE 64
char preadline_buff[PREADLINE_BUFF_SIZE] = {' '};

void preadline(void)
{
    //saving previous index on a screen
    uint32_t start_term_col = term_col;
    uint32_t start_term_row = term_row;
    uint32_t cmd_curr = 0, cmd_size = PREADLINE_BUFF_SIZE;
    uint8_t chr = 0, flags = 0;
    while((chr = kbd_getchar(&flags)) != '\n')
    {
        if(chr == 129 && cmd_curr > 0)
        {
            preadline_buff[--cmd_curr] = ' ';
        }
        else if(chr && chr != 129)
        {
            if(GET_BYTE(flags, 0))  chr -= 32;
            if(cmd_curr != cmd_size)
                preadline_buff[cmd_curr++] = chr;
        }
        term_print(preadline_buff);
        term_col = start_term_col;
        term_row = start_term_row;
    }
    term_print("\n");
}