#include "servo.h"
#include "pwm.h"
#include "config.h"

//define macros if not defined in config.h
#ifndef SERVO_MIN_US
#define SERVO_MIN_US     1000U
#endif
 
#ifndef SERVO_MAX_US
#define SERVO_MAX_US     2000U
#endif
 
#ifndef SERVO_CENTER_US
#define SERVO_CENTER_US  ((SERVO_MIN_US + SERVO_MAX_US) / 2U)
#endif

static uint16_t s_current_us = SERVO_CENTER_US;
 
static uint16_t servo_clamp_us(uint16_t us){
    if (us < SERVO_MIN_US) {
        return (uint16_t)SERVO_MIN_US;
    }
    if (us > SERVO_MAX_US) {
        return (uint16_t)SERVO_MAX_US;
    }
    return us;
}
 
void servo_init(void){
    pwm_init();
    servo_write_us(SERVO_CENTER_US);
}
 
void servo_write_us(uint16_t us)
{
    s_current_us = servo_clamp_us(us);
    pwm_set_pulse_us(s_current_us);
}
 
uint16_t servo_get_us(void){
    return s_current_us;
}