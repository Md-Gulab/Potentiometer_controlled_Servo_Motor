#include <stdint.h>

#include "app.h"

/* Crude busy-wait, roughly 1 ms per call at 16 MHz and -O0.
 - It is replaced by the TIM6 100 Hz tick in the next step. */
static void delay_crude_ms(uint32_t ms){
    for (volatile uint32_t i = 0U; i < ms * 2000U; i++) {
    }
}

int main(void){
    app_init();

    while (1) {
        app_step();
        delay_crude_ms(10U);    /* ~100 Hz control loop */
    }
}