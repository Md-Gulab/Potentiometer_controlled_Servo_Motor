#ifndef TICK_H
#define TICK_H

#include <stdbool.h>
#include <stdint.h>

/* Start TIM6 so that it raises an interrupt hz times per second.
 - hz must be between 16 and 1,000,000 (TIM6 is a 16-bit timer fed at 1 MHz). */
void tick_init(uint32_t hz);

/* Returns true once for every tick that has elapsed, false when none is pending.
 - Pending ticks are never lost: if the loop runs late, it catches up. */
bool tick_consume(void);

/* Total ticks since tick_init(). Handy for checking the rate in GDB:
 -   print g_tick_count                                                  */
extern volatile uint32_t g_tick_count;

#endif 