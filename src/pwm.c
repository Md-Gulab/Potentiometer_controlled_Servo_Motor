#include"stm32f4xx.h"
#include"pwm.h"

void pwm_init(){
    //enable the clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN ;
    //set the tick,16 MHz / 16 = 1 MHz and one tick = 1 µs.
    TIM3->PSC = 15 ;
    //set auto-reload value,giving 20ms
    TIM3->ARR = 19999 ;
    //set pwm mode
    TIM3->CCMR1 &= ~(0x00000078) ;
    TIM3->CCMR1 |=  (0x00000068) ;
    //auto-reload-preload
    TIM3->CR1 |= TIM_CR1_ARPE ;
    //output enable
    TIM3->CCER |= TIM_CCER_CC1E ;
    //initial pulse
    TIM3->CCR1 = 1500 ;
    //load registers
    TIM3->EGR |= TIM_EGR_UG ;
    //start
    TIM3->CR1 |= TIM_CR1_CEN ;
}

void pwm_set_pulse_us(uint16_t us){
    TIM3->CCR1 = us ;
}