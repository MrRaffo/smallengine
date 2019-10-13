#ifndef __timer_h__
#define __timer_h__

#include <stdint.h>

struct timer {
        uint32_t frame_rate;    // desired frame rate
        uint32_t frame_ms;      // milliseconds per frame
        uint32_t total_ms;      // total time since initialization
        uint32_t last_time;     // time stamp of last frame
        uint32_t cur_time;      // time stamp of current frame
        uint32_t delta_ms;      // milliseconds since last frame
        double delta_s;         // seconds since last frame
};

/*
 * Initialise the clock, returns 1 if successful
 * if 0 is passed, the timer will run as fast as it can
 */
struct timer timer_init(uint32_t frame_rate);

/*
 * update the timer and all values in the struct
 */
void timer_tic(struct timer *t);


#endif // __timer_h__
