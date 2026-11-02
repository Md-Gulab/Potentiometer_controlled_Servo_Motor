#include "filter.h"

//Do NOT include stm32f4xx.h here, so this file also builds
//with the PC's native gcc for unit tests

void filter_init(filter_t *f){
    for (uint32_t i = 0U; i < FILTER_LEN; i++) {
        f->buf[i] = 0U;
    }
    f->sum    = 0U;
    f->idx    = 0U;
    f->primed = false;
}

uint16_t filter_update(filter_t *f, uint16_t sample){
    if (!f->primed) {
        //First sample: fill the whole window with it
        for (uint32_t i = 0U; i < FILTER_LEN; i++) {
            f->buf[i] = sample;
        }
        f->sum    = (uint32_t)sample * FILTER_LEN;
        f->idx    = 0U;
        f->primed = true;
    } 
    else {
        //Replace the oldest sample and adjust the running sum
        f->sum -= f->buf[f->idx];
        f->buf[f->idx] = sample;
        f->sum += sample;
        f->idx++;
        if (f->idx >= FILTER_LEN) {
            f->idx = 0U;
        }
    }
    //Round to nearest instead of truncating
    return (uint16_t)((f->sum + (FILTER_LEN / 2U)) / FILTER_LEN);
}