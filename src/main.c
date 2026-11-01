#include "stm32f4xx.h"
#include "gpio.h"
#include "pwm.h"

static void delay_crude_ms(uint32_t ms)
{
    for (volatile uint32_t i = 0; i < ms * 2000U; i++) { }
}

int main(void)
{
    gpio_init();      /* PA6 = AF2 must be set before the timer output works */
    pwm_init();

    while (1) {
        pwm_set_pulse_us(1000);  delay_crude_ms(1000);   /* one end   */
        pwm_set_pulse_us(1500);  delay_crude_ms(1000);   /* center    */
        pwm_set_pulse_us(2000);  delay_crude_ms(1000);   /* other end */
        pwm_set_pulse_us(1500);  delay_crude_ms(1000);   /* center    */
    }
}