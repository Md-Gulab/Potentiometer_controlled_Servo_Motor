#ifndef ADC_H
#define ADC_H

#include <stdbool.h>
#include <stdint.h>

/* Initialise ADC1 for single software-triggered conversions on channel 0 (PA0).
 * PA0 must already be set to analog mode (see gpio.c). */
void adc_init(void);

/* Start one conversion and wait for the result.
 * Returns true and writes a 12-bit value (0..4095) to *value on success,
 * returns false if the conversion did not finish in time. */
bool adc_read(uint16_t *value);

#endif /* ADC_H */