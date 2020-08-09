#include <stdio.h>
#include <stdint.h>

#include <SDL2/SDL.h>

#include <smallengine/se_sys.h>

/*
 * Initialise the clock, returns 1 if successful
 */
struct timer timer_init(uint32_t frame_rate)
{
        struct timer t = {0, 0.0, 0, 0, 0, 0.0};

        if (SDL_InitSubSystem(SDL_INIT_TIMER) != 0) {
                SDL_Log("Unable to initialize SDL timer: %s", SDL_GetError());
                return t;
        }


        if (frame_rate <= 0) {
                t.frame_rate = 0;
                t.frame_ms = 0;
        } else {
                t.frame_rate = frame_rate;
                t.frame_ms = (int)(1000.0/(double)frame_rate);
        }

        t.cur_time = t.last_time = SDL_GetTicks();

        return t;
}

/*
 * update the timer and all values in the struct
 */
void timer_tic(struct timer *t)
{
        t->last_time = t->cur_time;

        t->cur_time = SDL_GetTicks();

        t->delta_ms = t->cur_time - t->last_time;

        // wait for the end of the frame if necessary
        if (t->frame_rate && (t->delta_ms < t->frame_ms)) {
                SDL_Delay(t->frame_ms - t->delta_ms);
                t->delta_ms = t->frame_ms;
                t->cur_time = t->last_time + t->frame_ms;
        }

        t->total_ms += t->delta_ms;
        t->delta_s = (double)(t->delta_ms)/1000.0;
}

