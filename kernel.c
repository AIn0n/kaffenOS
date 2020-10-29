#include <stddef.h>
#include_next <stdint.h>

#if defined(__linux__)
    #error "smth go wrong"
#elif !defined(__i386__)
    #error "smth go wrong"
#endif

volatile uint16_t* vga_buffer = (uint16_t *) 0xB8000;
const int VGA_COLS = 80;
const int VGA_ROWS = 25;

int term_col = 0;
int term_row = 0;

void term_init()
{
    size_t index;
    for(int col = 0; col < VGA_ROWS; ++col)
    {
        for(int row = 0; row < VGA_ROWS; ++row)
        {
            index = (VGA_COLS * row) + col;
            
        }
    }
}

int main (void) {return 0;}