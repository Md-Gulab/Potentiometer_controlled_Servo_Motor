#ifndef FILTER_H
#define FILTER_H

#include <stdbool.h>
#include <stdint.h>

/* Number of samples in the moving average. Tune here (keep it the only
 - definition: remove FILTER_LEN from config.h if you also have it there) */
#ifndef FILTER_LEN
#define FILTER_LEN  8U
#endif

#if (FILTER_LEN < 1U)
#error "FILTER_LEN must be at least 1"
#endif

typedef struct {
    uint16_t buf[FILTER_LEN];   // last FILTER_LEN samples (circular)      
    uint32_t sum;               // running sum of buf[]                    
    uint8_t  idx;               // next slot to overwrite                  
    bool     primed;            // false until the first sample arrives    
} filter_t;

/* Reset the filter. The next sample fills the whole window, so the output
 -starts at the first reading instead of ramping up from zero. */
void filter_init(filter_t *f);

//Feed one new sample, get the current moving average (rounded to nearest)
uint16_t filter_update(filter_t *f, uint16_t sample);

#endif 