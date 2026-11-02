#include "app.h"

#include <stdbool.h>
#include <stdint.h>

#include "adc.h"
#include "config.h"
#include "filter.h"
#include "gpio.h"
#include "map.h"
#include "servo.h"

/* Tuning values. Normally these live in config.h; the fallbacks only exist so
 - this file compiles before you have added them. Remove them once they are in
 - config.h. */
#ifndef ADC_IN_MIN
#define ADC_IN_MIN       10
#endif

#ifndef ADC_IN_MAX
#define ADC_IN_MAX       4085
#endif

#ifndef DEADBAND
#define DEADBAND         5
#endif

#ifndef SERVO_MIN_US
#define SERVO_MIN_US     1000
#endif

#ifndef SERVO_MAX_US
#define SERVO_MAX_US     2000
#endif

/* Debug variables: add them to the Cortex-Debug live watch, or `print` them in
 - GDB, to see the signal at each stage while tuning. */
volatile uint16_t g_dbg_raw;
volatile uint16_t g_dbg_filtered;
volatile uint16_t g_dbg_pulse_us;

static filter_t s_filter;
static uint16_t s_last_applied;     // filtered value that produced the last command 
static bool     s_have_applied;

void app_init(void){
    gpio_init();            // PA0 analog (pot), PA6 = AF2 (servo PWM) 
    adc_init();
    servo_init();           // starts PWM, servo goes to centre 
    filter_init(&s_filter);
    s_have_applied = false;
}

void app_step(void){
    uint16_t raw;

    // 1-> Sense: If the ADC fails, keep the servo where it is
    if (!adc_read(&raw)) {
        return;
    }
    g_dbg_raw = raw;

    // 2-> Process: smooth the reading
    uint16_t filtered = filter_update(&s_filter, raw);
    g_dbg_filtered = filtered;

    /* 3-> Deadband: ignore tiny changes so the servo does not hunt
     *     Always update at the ends so the full range stays reachable. */
    bool at_end = (filtered <= ADC_IN_MIN) || (filtered >= ADC_IN_MAX);
    int32_t change = (int32_t)filtered - (int32_t)s_last_applied;
    if (change < 0) {
        change = -change;
    }

    if (s_have_applied && !at_end && change <= DEADBAND) {
        return;
    }

    s_last_applied = filtered;
    s_have_applied = true;

    // 4-> Actuate: scale to a pulse width. servo_write_us() clamps it again
    int32_t pulse = map_range((int32_t)filtered, ADC_IN_MIN, ADC_IN_MAX,
                              SERVO_MIN_US, SERVO_MAX_US);
    g_dbg_pulse_us = (uint16_t)pulse;
    servo_write_us((uint16_t)pulse);
}