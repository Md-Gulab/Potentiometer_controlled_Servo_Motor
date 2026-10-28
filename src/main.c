#include "stm32f4xx.h"
#include "gpio.h"
#include "adc.h"

volatile uint16_t g_raw;

int main(void)
{
    gpio_init();
    adc_init();

    while (1) {
        adc_read((uint16_t *)&g_raw);
    }
}