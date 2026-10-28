#include<stm32f446xx.h>

void gpio_init(){
    //enable the clock for GPIOA
    RCC->AHB1ENR |= (0x00000001) ;

    //set "PA0" the bit 0&1 of GPIOA_MODER
    GPIOA->MODER &=  ~(0x00000003) ; // cleared
    GPIOA->MODER |=   (0x00000003) ; // set to 11

    //set "PA6" first as alternate funtion mode to be driven by TIM3
    GPIOA->AFR[0]  &= ~(0xFU << 24);   // clear PA6's AF field
    GPIOA->AFR[0]  |=  (2U   << 24);   // AF2 = TIM3
    GPIOA->MODER   &= ~(3U   << 12);   // clear PA6's mode
    GPIOA->MODER   |=  (2U   << 12);   // alternate function
}



