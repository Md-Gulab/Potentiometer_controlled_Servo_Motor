#include <stdint.h>

#include "stm32f4xx.h"      // for __WFI() 

#include "app.h"
#include "config.h"
#include "tick.h"

#ifndef LOOP_HZ
#define LOOP_HZ     100U
#endif

int main(void){
    app_init();
    tick_init(LOOP_HZ);

    while (1) {
        // Sleep until the next tick, then run exactly one control step
        while (!tick_consume()) {
            __WFI();
        }
        app_step();
    }
}