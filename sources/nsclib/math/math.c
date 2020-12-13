#include "math.h"
#include <stdint.h>


int32_t
pow_rec(int32_t a, uint32_t b) {return ((b) ? a * pow_rec(a, --b) : 1);}