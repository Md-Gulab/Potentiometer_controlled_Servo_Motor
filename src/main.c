#include "stm32f4xx.h"
#include "gpio.h"
#include "pwm.h"
#include "servo.h"

static void delay_crude_ms(uint32_t ms){
    for (volatile uint32_t i = 0; i < ms * 2000U; i++) { }
}

int main(void){
    gpio_init();      /* PA6 = AF2 must be set before the timer output works */
    servo_init();

    while (1) {
    servo_write_us(500);   delay_crude_ms(1000);   /* should go to the 1000 end only */
    servo_write_us(3000);  delay_crude_ms(1000);
    }

}    