#ifndef SERVO_H
#define SERVO_H

#include<stdint.h>

//Start the PWM output and move the servo to the centre position
void servo_init(void) ;

//The 'us' value is clamped to SERVO_MIN_US..SERVO_MAX_US
void servo_write_us(uint16_t us);

//get the last pulse width that was actually applied (after clamping)
uint16_t servo_get_us(void);

#endif