#include <stdio.h>
#include <unistd.h>     // for sleep()
#include <assert.h>

#include <SDL2/SDL.h>

#include <smallengine/sys/timer.h>
#include <smallengine/maths/maths.h>

void TST_TimerInit()
{
        struct timer t = timer_init(60);
        assert(t.frame_rate == 60);
        assert(t.frame_ms == 16);

        assert(t.total_ms == 0);
        assert(t.delta_ms == 0);
        assert(t.cur_time == t.last_time);
        assert(double_equal(t.delta_s, 0.0));

        printf("[Timer Init] Complete, all tests pass!\n");
}

void TST_TimerTic()
{
        struct timer t = timer_init(60);
        timer_tic(&t);

        assert(t.cur_time == (t.last_time + t.delta_ms));
        assert(double_equal(t.delta_s, (double)t.delta_ms/1000.0) == 1);

        printf("[Timer Tic] Complete, all tests pass!\n");
}

int main()
{
        TST_TimerInit();
        TST_TimerTic();

        SDL_Quit();

        return 0;
}
