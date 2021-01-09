#include <stdint.h>
#include "terminal.h"
#include "keyboard.h"
#include "math.h"
#include "misc.h"
#include "string.h"

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

void
term_init()
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

void 
term_putc(char c, uint8_t fg)
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
            vga_buffer[index] = vga_entry(c, fg, 0);
            ++term_col;
            break;
        }
    }
    if(term_col >= VGA_COLS){   term_col =0;    ++term_row;     }
    if(term_row >= VGA_ROWS){   term_col = 0;   term_row = 0;   }
};

void 
term_print(const char* str){
    if(str == NULL) return;
    
    int32_t size = strlen(str);
    if(size == -1) return;
    
    int32_t get_int = -1;
    uint8_t color = VGA_WHITE;

    for(uint32_t i = 0; i < size; ++i) 
    {
        if(str[i] == '$')
        {
            get_int = i;
            continue;
        }
        if(get_int != -1)
        {
            color = atoi((str + i));
            if(color != -1) { i += uint32_len(color,10) - 1; }
            else
            {
                color = VGA_WHITE;
                i = get_int;
            }
            get_int = -1;
            continue;
        }
        if(str[i] == ' ' || str[i] == '\n') color = VGA_WHITE;
        term_putc(str[i], color);
    }
}

void 
term_print_int32(int32_t a)
{
    uint8_t minus = 0;
    int32_t b = a;
    uint16_t len = 0;

    if( a < 0) {minus = 1; a = -(a);}   //checking the sign
    len = uint32_len(a, 10);          //finding length
    if(minus) term_putc('-', VGA_WHITE);           //putting "-" if a < 0
    for(uint16_t i = len; i > 0; --i)    //putting every char in for loop
    {
        b = a;
        b /= (int)pow_rec(10, i - 1);
        b %= 10;
        term_putc(b + '0', VGA_WHITE);
    }
}

void
term_print_uint32(uint32_t a, uint8_t base)
{
    int32_t b = a;
    uint16_t len = 0;
    len = uint32_len(b, base);          //finding length
    for(uint16_t i = len; i > 0; --i)   //putting every char in for loop
    {
        b = a;
        b /= pow_rec(base, i - 1);
        b %= base;
        term_putc(b + ((b>9) ? '7' : '0'), VGA_WHITE);
    }
}

void
term_print_int_at(uint32_t a, uint8_t base, uint8_t x, uint8_t y)
{
    if(x > VGA_COLS || y > VGA_ROWS) return;
    uint32_t start_term_col = term_col;
    uint32_t start_term_row = term_row;
    term_row = y;
    term_col = x;

    term_print_uint32(a, base);

    term_col = start_term_col;
    term_row = start_term_row;
}

//--------------primitive readline-------

#define PREADLINE_BUFF_SIZE 64
char preadline_buff[PREADLINE_BUFF_SIZE] = {'\0'};

char* 
preadline(void)
{
    //saving previous index on a screen
    uint32_t start_term_col = term_col;
    uint32_t start_term_row = term_row;
    uint32_t cmd_curr = 0, cmd_size = PREADLINE_BUFF_SIZE;
    uint8_t chr = 0, flags = 0;
    preadline_flush();
    while((chr = kbd_getchar(&flags)) != '\n')
    {
        if(chr == 129 && cmd_curr > 0)
        {
            preadline_buff[--cmd_curr] = ' ';
        }
        else if(chr && chr != 129)
        {
            if(GET_BYTE(flags, 0) == TRUE)  chr -= 32;
            if(GET_BYTE(flags, 2) == TRUE && chr == 'u')
            {
                preadline_flush();
                cmd_curr = 0;
            }
            else if(cmd_curr != cmd_size) { preadline_buff[cmd_curr++] = chr; }
        }
        term_print(preadline_buff);
        term_col = start_term_col;
        term_row = start_term_row;
    }
    preadline_buff[cmd_curr] = '\0';
    term_print("\n");
    return preadline_buff;
}

void preadline_flush(void) {memset(preadline_buff, ' ', PREADLINE_BUFF_SIZE);}