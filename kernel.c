#include <stddef.h>
#include_next <stdint.h>

#if defined(__linux__)
    #error "smth go wrong"
#elif !defined(__i386__)
    #error "smth go wrong"
#endif

//for vga display
volatile uint16_t* vga_buffer = (uint16_t *) 0xB8000;
const int VGA_COLS = 80;
const int VGA_ROWS = 25;

//for terminal
int term_col;
int term_row;

static inline uint16_t vga_entry(unsigned char uc, uint8_t fg, uint8_t bg)
{
    uint16_t color = fg | bg << 4;
    return (uint16_t) color << 8 | (uint16_t) uc;
}

void term_init()
{
    term_col = 0; term_row = 0;
    for(int col = 0; col < VGA_COLS; ++col)
    {
        for(int row = 0; row < VGA_ROWS; ++row)
        {
            const size_t index = (VGA_COLS * row) + col;
            //VGA_CHAR BBBBFFFFCCCCCCCC, B-background F-foreground C-ASCII char
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
            const size_t index = (VGA_COLS * term_row) + term_col;
            vga_buffer[index] = vga_entry(c, 15, 0);
            ++term_col;
            break;
        }
    }
    if(term_col >= VGA_COLS){   term_col =0;    ++term_row;     }
    if(term_row >= VGA_ROWS){   term_col = 0;   term_row = 0;   }
}

void term_print(const char* str){
    for(size_t i = 0; str[i] != '\0'; ++i) term_putc(str[i]);
}

void kernel_main (void) 
{
    term_init();
    term_print("kaffenOS\nNIEDZIELA WIECZOR I HUMOR POPSUTY");
}