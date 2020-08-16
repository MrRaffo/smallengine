#include "gtest/gtest.h"
#include "smallengine/se_maths.h"

TEST(MathsTest, DoubleEqualTest) {
    EXPECT_EQ(double_equal(0.0001, 0.0001), 1);
    EXPECT_EQ(double_equal(0.0001, 0.0002), 0);
    // numbers of negligible difference should be considered equal
    double x = 0.000000001;
    EXPECT_EQ(double_equal(x, x+(EPSILON/2.0)), 1);
}

TEST(MathsTest, DoubleCompareTest) {
    EXPECT_EQ(double_compare(1.0, 0.99999), 0);
    EXPECT_EQ(double_compare(1.0, 0.5), 1);
    EXPECT_EQ(double_compare(0.5, 0.99999), -1);
}

TEST(MathsTest, TupleInitTest) {
    
    struct tuple t = tuple(1.0, 2.0, 3.0, 4.0);
    EXPECT_EQ(double_equal(t.x, 1.0), 1);
    EXPECT_EQ(double_equal(t.y, 2.0), 1);
    EXPECT_EQ(double_equal(t.z, 3.0), 1);
    EXPECT_EQ(double_equal(t.w, 4.0), 1);

    point p = point_3d(2.5, 3.5, 4.5);
    EXPECT_EQ(double_equal(p.z, 4.5), 1);
    EXPECT_EQ(double_equal(p.w, 1.0), 1);

    vector v = vector_3d(2.5, 2.5, 1.5);
    EXPECT_EQ(double_equal(v.y, 2.5), 1);
    EXPECT_EQ(double_equal(v.w, 0.0), 1);

    point p2d = point_2d(0.1, 0.1);
    EXPECT_EQ(double_equal(p2d.z, 0.0), 1);
    EXPECT_EQ(double_equal(p2d.x, 0.1), 1);

    vector v2d = vector_2d(0.003, 0.5);
    EXPECT_EQ(double_equal(v2d.y, 0.5), 1);
}

TEST(MathsTest, TupleEqualTest) {
    
    struct tuple t1 = tuple(0.00000001, 0.5, 0.3, 2.5);
    struct tuple t2 = tuple(0.00000002, 0.499999999, 0.3, 2.5);
    EXPECT_EQ(tuple_equal(t1, t2), 1);

    point p1 = point_3d(1.0, 1.0, 1.0);
    point p2 = point_3d(1.0, 1.0, 1.0);
    EXPECT_EQ(tuple_equal(p1, p2), 1);

    vector v1 = vector_2d(0.3, 0.4);
    vector v2 = vector_2d(0.29999999, 0.4);
    EXPECT_EQ(tuple_equal(v1, v2), 1);

    vector v3 = vector_2d(0.1, 0.1);
    vector v4 = vector_2d(90.0, 100.0);
    EXPECT_EQ(tuple_equal(v3, v4), 0);
}
