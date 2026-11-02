#include "tick.h"
#include "stm32f4xx.h"

/* TIM6 sits on APB1. After reset the chip runs from the 16 MHz HSI with no
 - prescalers, so the timer clock is 16 MHz. If you change the clock tree later
 - (PLL), update this value. */
#ifndef TICK_TIMER_CLK_HZ
#define TICK_TIMER_CLK_HZ   16000000UL
#endif

#define TICK_BASE_HZ        1000000UL      // timer counts at 1 MHz (1 tick = 1 us) 

volatile uint32_t g_tick_count;            // incremented by the ISR              
static uint32_t   s_seen;                  // ticks already handed to the caller  

void tick_init(uint32_t hz){
    if (hz < 16U) {
        hz = 16U;                          // ARR is 16 bits wide: 1 MHz / 65536 
    }
    if (hz > TICK_BASE_HZ) {
        hz = (uint32_t)TICK_BASE_HZ;
    }

    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    (void)RCC->APB1ENR;                    // read back: short delay after clock enable 

    TIM6->CR1  = 0U;                       // stop while configuring 
    TIM6->PSC  = (TICK_TIMER_CLK_HZ / TICK_BASE_HZ) - 1U;   // 16 MHz -> 1 MHz 
    TIM6->ARR  = (TICK_BASE_HZ / hz) - 1U;                  // 100 Hz -> 9999  

    TIM6->EGR  = TIM_EGR_UG;               // load PSC and ARR now             
    TIM6->SR   = 0U;                       // clear the UIF flag that UG raised 

    TIM6->DIER = TIM_DIER_UIE;             // update interrupt on 

    NVIC_SetPriority(TIM6_DAC_IRQn, 5U);
    NVIC_EnableIRQ(TIM6_DAC_IRQn);

    g_tick_count = 0U;
    s_seen       = 0U;

    TIM6->CR1 = TIM_CR1_CEN;               // go 
}

/* The name must match the vector table exactly. It overrides the weak default
 - handler from the startup file. */
void TIM6_DAC_IRQHandler(void){
    if ((TIM6->SR & TIM_SR_UIF) != 0U) {
        TIM6->SR &= ~TIM_SR_UIF;           // clear first, or the ISR re-triggers forever 
        g_tick_count++;
    }
}

bool tick_consume(void){
    if (s_seen != g_tick_count) {
        s_seen++;
        return true;
    }
    return false;
}