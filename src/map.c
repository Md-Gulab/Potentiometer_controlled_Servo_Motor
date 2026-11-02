#include "map.h"

//Do NOT include stm32f4xx.h here, so this file also builds
//with the PC's native gcc for unit tests
int32_t map_range(int32_t x, int32_t in_min, int32_t in_max,
                  int32_t out_min, int32_t out_max){
    if (in_min == in_max) {
        return out_min;
    }

    //Clamp x to the input range (works even if the range is reversed)
    int32_t lo = (in_min < in_max) ? in_min : in_max;
    int32_t hi = (in_min < in_max) ? in_max : in_min;

    if (x < lo) {
        x = lo;
    }
    if (x > hi) {
        x = hi;
    }

    return out_min + ((x - in_min) * (out_max - out_min)) / (in_max - in_min);
}