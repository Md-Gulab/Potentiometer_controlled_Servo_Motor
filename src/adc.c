#include "adc.h"
#include "stm32f4xx.h"

/* Upper bound on polling iterations. One conversion takes ~1000 CPU cycles at
 * 16 MHz, so this is far more than enough while still preventing a hang. */
#define ADC_TIMEOUT_LOOPS   100000U

void adc_init(void){
    // Clock for ADC1 (APB2) 
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    // ADC clock prescaler (common register): 00 = PCLK2 / 2 = 8 MHz 
    ADC123_COMMON->CCR &= ~ADC_CCR_ADCPRE;

    // 12-bit resolution, scan mode off 
    ADC1->CR1 &= ~(ADC_CR1_RES | ADC_CR1_SCAN);

    // Single conversion, right-aligned, software trigger 
    ADC1->CR2 &= ~(ADC_CR2_CONT | ADC_CR2_ALIGN | ADC_CR2_EXTEN);

    // Sample time for channel 0: 111 = 480 cycles (pot has high impedance) 
    ADC1->SMPR2 |= ADC_SMPR2_SMP0;

    // Regular sequence: 1 conversion (L = 0), first (and only) channel = 0 
    ADC1->SQR1 &= ~ADC_SQR1_L;
    ADC1->SQR3 &= ~ADC_SQR3_SQ1;

    //  Power on the ADC and give it time to stabilise (tSTAB ~ 3 us) 
    ADC1->CR2 |= ADC_CR2_ADON;
    for (volatile uint32_t i = 0; i < 100U; i++) {
        //DELAY- do nothing
    }
}

bool adc_read(uint16_t *value){
    uint32_t timeout = ADC_TIMEOUT_LOOPS;

    // Start conversion (only works while ADON = 1) 
    ADC1->CR2 |= ADC_CR2_SWSTART;

    // Wait for end-of-conversion flag, with timeout 
    while ((ADC1->SR & ADC_SR_EOC) == 0U) {
        if (--timeout == 0U) {
            return false;
        }
    }

    // Reading DR also clears EOC 
    *value = (uint16_t)ADC1->DR;
    return true;
}