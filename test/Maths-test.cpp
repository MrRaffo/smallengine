#include "gtest/gtest.h"
#include "smallengine/se_maths.h"

TEST(MathsTest, double_equal_test) {
    EXPECT_EQ(double_equal(0.0001, 0.0001), 1);
    EXPECT_EQ(double_equal(0.0001, 0.0002), 0);
    EXPECT_EQ(double_equal(0.00000001, 0.00000002), 1);
}
