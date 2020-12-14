#include "math.h"
#include <stdint.h>


int32_t
pow_rec(int32_t a, uint32_t b) {return ((b) ? a * pow_rec(a, --b) : 1);}

uint16_t
uint32_len(uint32_t val, uint8_t base)
{
    uint16_t len = 0;
    do { val /= base; ++len;}while(val > 0);
    return len;
}