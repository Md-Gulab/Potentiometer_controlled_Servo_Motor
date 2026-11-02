#ifndef MAP_H
#define MAP_H

#include <stdint.h>

/* Linearly scale x from [in_min, in_max] to [out_min, out_max].
 - x is clamped to the input range first, so the result never leaves the
   output range.
 - Either range may be reversed (min > max).
 - If in_min == in_max the result is out_min (no division by zero).
 - Integer maths only. The intermediate product must fit in 32 bits, which is
   true for 12-bit ADC values mapped to servo pulse widths. */
int32_t map_range(int32_t x, int32_t in_min, int32_t in_max,
                  int32_t out_min, int32_t out_max);

#endif 