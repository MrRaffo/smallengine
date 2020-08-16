#include "gtest/gtest.h"
#include "smallengine/se_sys.h"
#include "smallengine/se_maths.h"

TEST(TimerTest, TimerInitTest) {
    
    struct timer t = timer_init(60);
    EXPECT_EQ(t.frame_rate, 60);
    EXPECT_EQ(t.frame_ms, 16);

    EXPECT_EQ(t.total_ms, 0);
    EXPECT_EQ(t.delta_ms, 0);
    EXPECT_EQ(t.cur_time, t.last_time);
    EXPECT_EQ(double_equal(t.delta_s, 0.0), 1);
}

TEST(TimerTest, TimerTicTest) {
    struct timer t = timer_init(60);
    timer_tic(&t);

    EXPECT_EQ(t.cur_time, (t.last_time + t.delta_ms));
    EXPECT_EQ(double_equal(t.delta_s, (double)t.delta_ms/1000.0), 1);
}
