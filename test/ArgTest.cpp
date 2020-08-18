#include "gtest/gtest.h"
#include "smallengine/se_sys.h"

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
    EXPECT_EQ(arg_check(arg3), 0);
}
