#include "gtest/gtest.h"
#include "smallengine/se_sys.h"
#include "smallengine/se_maths.h"

/*=================
 * ARG TESTS
 *===============*/

TEST(ArgTest, ArgCheckTest) {
    char *arglist[] = {"progname", "-test", "-another"};
    int argcnt = 3;

    arg_init(argcnt, arglist);

    std::string str1 = "-test";
    std::string str2 = "-another";
    std::string str3 = "-noarg";

    const char *arg1 = str1.c_str();
    const char *arg2 = str2.c_str();
    const char *arg3 = str3.c_str();

    EXPECT_EQ(arg_check(arg1), 1);
    EXPECT_EQ(arg_check(arg2), 2);
    EXPECT_EQ(arg_check(arg3), -1);
}

TEST(ArgTest, ArgGetValueTest) {
    char *arglist[] = {"progname", "--value", "100", "--another", "200", "--noval"};
    int argcnt = 6;

    arg_init(argcnt, arglist);

    std::string str1 = "--value";
    std::string str2 = "--another";
    std::string str3 = "--noval";

    const char *arg1 = str1.c_str();
    const char *arg2 = str2.c_str();
    const char *arg3 = str3.c_str();

    EXPECT_EQ(strncmp(arg_get_value("--value"), "100", 10), 0);
    EXPECT_EQ(strncmp(arg_get_value("--another"), "200", 10), 0);
    EXPECT_TRUE(arg_get_value("--noval") == nullptr);
    EXPECT_TRUE(arg_get_value("--noarg") == NULL);
}

/*==================
 * TIMER TESTS
 *================*/

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

/*==================
 * MEMORY TESTS
 *================*/

TEST(MemoryTest, MemTest) {
        mem_init(1024);
        //mem_print_report();

        EXPECT_EQ(mem_total(), 1024);
        EXPECT_EQ(mem_used(), 32);

        void *ptr1 = mem_alloc(256);
        EXPECT_EQ(mem_used(), 320);

        void *ptr2 = mem_alloc(128);
        void *ptr3 = mem_alloc(64);
        void *ptr4 = mem_alloc(200);
        //mem_print_report();

        EXPECT_EQ(mem_used(), 808);
        //mem_print_report();

        // test freeing memory
        //printf("Freeing pointer 3:\n");
        mem_free(ptr3);
        mem_free(ptr3);
        //mem_print_report();

        EXPECT_EQ(mem_used(), 744);

        //printf("Freeing pointer 2:\n");
        mem_free(ptr2);
        //mem_print_report();

        EXPECT_EQ(mem_used(), 584);
        
        //printf("Freeing pointer 4:\n");
        mem_free(ptr4);
        //mem_print_report();

        EXPECT_EQ(mem_used(), 320);
        
        //printf("Freeing pointer 1:\n");
        mem_free(ptr1);
        //mem_print_report();

        EXPECT_EQ(mem_used(), 32);
        EXPECT_EQ(mem_available(), 992);
        //mem_dump();

        mem_destroy();
}

/*
TEST(MemoryTest, MemoryIntegrityTest)
{
        mem_free_all();
        
        int *ptr1 = (int *)mem_alloc(128);
        int *ptr2 = (int *)mem_alloc(128);
        void *ptr3 = mem_alloc(128);
        void *ptr4 = mem_alloc(128);

        // corrupt memory
        int *ptr5 = (int *)mem_alloc(64);
        int i = 0;
        for (i = 0; i < 5; i++) {
                ptr1[i] = 0xffffff;
                printf("Writing: %p\n", &ptr1[i]);
        }
        
        for (i = 0; i < 40; i++) {
                ptr2[i] = 0xffffff;
                printf("Writing: %p\n", &ptr2[i]);
        }
        printf("POINTERS: %p, %p\n", ptr1, ptr2);
        //printf("%p\n", ptr3);

        printf("VALUES: %d, %d\n", ptr1[3], ptr2[2]);

        mem_free(ptr3);
        mem_free(ptr4);
        mem_free(ptr5);

        mem_dump();
        mem_print_report();
}
*/

