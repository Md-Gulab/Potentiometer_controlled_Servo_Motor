#ifndef APP_H
#define APP_H

// Initialise every module (GPIO, ADC, servo/PWM, filter)
void app_init(void);

/* One control iteration: read the pot, filter it, apply the deadband,
 - map it to a pulse width and command the servo.
 - Call it at a fixed rate (100 Hz in this project). */
void app_step(void);

#endif 