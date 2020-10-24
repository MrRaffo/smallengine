#include <math.h>

#include "gtest/gtest.h"
#include "smallengine/se_maths.h"

/**
 * MATHS TESTS
 */

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

/**
 * TUPLE TESTS
 */

TEST(TupleTest, TupleInitTest) {
    
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

TEST(TupleTest, TupleEqualTest) {
    
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

TEST(TupleTest, TupleAddTest) {
    
    struct tuple t1 = tuple(1.0, 1.0, 1.0, 1.0);
    struct tuple t2 = tuple(0.5, 1.5, 2.5, 3.5);
    struct tuple t3 = tuple(1.5, 2.5, 3.5, 4.5);
    struct tuple t4 = tuple_add(t1, t2);

    EXPECT_EQ(tuple_equal(t3, t4), 1);

    point p = point_3d(1.0, 0.49999999, 0.0);
    vector v = vector_3d(1.0, 0.5, 0.0);
    point r = point_3d(2.0, 1.0, 0.0);
    point r2 = tuple_add(p, v);

    EXPECT_EQ(tuple_equal(r, r2), 1);
}

TEST(TupleTest, TupleSubtractTest) {

    struct tuple t1 = tuple(1.0, 1.0, 1.0, 1.0);
    struct tuple t2 = tuple(0.5, 1.5, 2.5, 3.5);
    struct tuple t3 = tuple(0.5, -0.5, -1.5, -2.5);
    struct tuple t4 = tuple_subtract(t1, t2);
        
    EXPECT_EQ(tuple_equal(t3, t4), 1);

    point p = point_3d(1.0, 0.49999999, 0.0);
    vector v = vector_3d(1.0, 0.5, 0.0);
    point r = point_3d(0.0, 0.0, 0.0);
    point r2 = tuple_subtract(p, v);

    EXPECT_EQ(tuple_equal(r, r2), 1);
}

TEST(TupleTest, TupleNegateTest) {
    struct tuple t1 = tuple(0.0, 1.0, 2.0, 3.0);
    struct tuple t2 = tuple(0.0, -1.0, -2.0, -3.0);
    EXPECT_EQ(tuple_equal(tuple_negate(t1), t2), 1);
}

TEST(TupleTest, TupleScaleTest) {
    struct tuple t1 = tuple(1.0, 1.0, 1.0, 1.0);
    struct tuple t2 = tuple(10.0, 10.0, 10.0, 10.0);
    EXPECT_EQ(tuple_equal(tuple_scale(t1, 10.0), t2), 1);
}

TEST(TupleTest, TupleDivideTest) {
    struct tuple t1 = tuple(1.0, 1.0, 1.0, 1.0);
    struct tuple t2 = tuple(10.0, 10.0, 10.0, 10.0);
    EXPECT_EQ(tuple_equal(tuple_divide(t2, 10.0), t1), 1);
}

TEST(TupleTest, VectorMagnitudeTest) {
        vector v1 = vector_3d(1.0, 2.0, 3.0);
        vector v2 = vector_3d(3.0, 4.0, 5.0);

        EXPECT_EQ(double_equal(vector_magnitude(v1), sqrt(14.0)), 1);
        EXPECT_EQ(double_equal(vector_magnitude(v2), sqrt(50.0)), 1);
}

TEST(TupleTest, VectorCompareTest) {
        vector v1 = vector_3d(1.0, 2.0, 3.0);
        vector v2 = vector_3d(3.0, 4.0, 5.0);
        vector v3 = vector_3d(-1.0, -2.0, -3.0);
        vector v4 = vector_3d(-10.0, -10.0, -10.0);

        EXPECT_EQ(vector_compare(v1, v2), -1);
        EXPECT_EQ(vector_compare(v2, v1), 1);
        EXPECT_EQ(vector_compare(v1, v3), 0);
        EXPECT_EQ(vector_compare(v4, v1), 1);
}

TEST(TupleTest, VectorMagnitudeCompareTest) {
        vector v = vector_3d(1.0, 2.0, 3.0);
        double m = sqrt(14.0);

        EXPECT_EQ(vector_magnitude_compare(v, m), 0);
        EXPECT_EQ(vector_magnitude_compare(v, 1.0), 1);
        EXPECT_EQ(vector_magnitude_compare(v, 100.0), -1);
}

TEST(TupleTest, VectorNormalTest) {
        struct tuple v1 = vector_3d(1.5f, 5.0f, 0.0f);
        struct tuple v2 = vector_3d(0.2f, 1.8f, 6.2f);
        struct tuple v3 = vector_3d(-8.65f, -6.234f, 1.0f);
        struct tuple v4 = vector_3d(2.0f, 3.0f, 4.0f);
        struct tuple v5 = vector_3d(1.0f, 2.0f, 3.0f);
        struct tuple v6 = vector_3d(45.3f, -90.123f, 3.3f);
        struct tuple v7 = vector_3d(0.0f, 0.0f, 0.0f);       // special case

        EXPECT_EQ(double_equal(vector_magnitude(vector_normal(v1)), 1.0f), 1);
        EXPECT_EQ(double_equal(vector_magnitude(vector_normal(v2)), 1.0f), 1);
        EXPECT_EQ(double_equal(vector_magnitude(vector_normal(v3)), 1.0f), 1);
        EXPECT_EQ(double_equal(vector_magnitude(vector_normal(v4)), 1.0f), 1);
        EXPECT_EQ(double_equal(vector_magnitude(vector_normal(v5)), 1.0f), 1);
        EXPECT_EQ(double_equal(vector_magnitude(vector_normal(v6)), 1.0f), 1);
        EXPECT_EQ(tuple_equal(vector_normal(v7), v7), 1);    // just don't break...
}

TEST(TupleTest, VectorDotTest) {
        struct tuple v1 = vector_3d(1.0f, 2.0f, 3.0f);
        struct tuple v2 = vector_3d(2.0f, 3.0f, 4.0f);
        struct tuple v3 = vector_3d(3.0f, -3.0f, 3.0f);

        EXPECT_EQ(double_equal(vector_dot(v1, v2), 20.0f), 1);
        EXPECT_EQ(double_equal(vector_dot(v1, v3), 6.0f), 1);
        EXPECT_EQ(double_equal(vector_dot(v2, v3), 9.0f), 1);
        EXPECT_EQ(double_equal(vector_dot(v3, v3), 27.0f), 1);
}

TEST(TupleTest, VectorCrossTest) {
        struct tuple a = vector_3d(1.0f, 0.0f, 0.0f);
        struct tuple b = vector_3d(0.0f, 1.0f, 0.0f);
        struct tuple c = vector_3d(0.0f, 0.0f, 1.0f);

        EXPECT_EQ(tuple_equal(vector_cross(a, b), c), 1);
        EXPECT_EQ(tuple_equal(vector_cross(b, a), tuple_negate(c)), 1);
        EXPECT_EQ(tuple_equal(vector_cross(c, a), b), 1);
        EXPECT_EQ(tuple_equal(vector_cross(b, c), a), 1);
}

// test reflection of vector against given surface normal
TEST(TupleTest, VectorReflectTest) {
        struct tuple vec = vector_3d(1.0, -1.0, 0.0);
        struct tuple nor = vector_3d(0.0, 1.0, 0.0);
        struct tuple ref = vector_reflect(vec, nor);

        EXPECT_EQ(tuple_equal(ref, vector_3d(1.0, 1.0, 0.0)), 1);

        // reflect vertical drop against 45 degree incline should produce
        // horizontal vector
        vec = vector_3d(0.0, -1.0, 0.0);
        nor = vector_3d(sqrt(2.0)/2.0, sqrt(2.0)/2.0, 0.0);
        ref = vector_reflect(vec, nor);

        EXPECT_EQ(tuple_equal(ref, vector_3d(1.0, 0.0, 0.0)), 1);
}

/**
 * MATRIX TESTS
 */

// matrix initialisation
TEST(MatrixTest, MatrixInitTest) {

    struct matrix mat = matrix(5, 4);

    EXPECT_EQ(mat.row, 5);
    EXPECT_EQ(mat.col, 4);
}

TEST(MatrixTest, MatrixEqualTest) {

    struct matrix m = matrix(4, 3);
    struct matrix n = matrix(4, 3);
    struct matrix p = matrix(3, 4);

    EXPECT_EQ(matrix_equal(m, n), 1);
    EXPECT_EQ(matrix_equal(n, m), 1);
    EXPECT_EQ(matrix_equal(m, p), 0);

    // checking this is 1 just confirms it was successful before next test
    EXPECT_EQ(matrix_set(m, 1, 1, 3.5), 1);
    EXPECT_EQ(matrix_equal(m, n), 0);

    EXPECT_EQ(matrix_set(n, 1, 1, 3.4999999999), 1);
    EXPECT_EQ(matrix_equal(m, n), 1);
}

// test get and set together
TEST(MatrixTest, MatrixSetGetTest) {

    struct matrix m = matrix(2, 2);
    EXPECT_EQ(matrix_set(m, 1, 1, 1000.500011), 1);

    EXPECT_EQ(matrix_get(m, 3, 5), 0.0);
    EXPECT_EQ(matrix_get(m, 0, 0), 0.0);
    EXPECT_EQ(double_equal(matrix_get(m, 1, 1), 1000.500011), 1);

    EXPECT_EQ(matrix_set(m, 5, 5, 0.5), 0);
} 

// matrix copy
TEST(MatrixTest, MatrixCopyTest) {
    
    struct matrix m = matrix(3, 3);
    double data[] = {3.0, 30.0, 1.0,
                     1.0, 6.0, 10.0,
                     0.0, 0.5, 4.0};
    m.matrix = data;

    struct matrix copy = matrix_copy(m);

    EXPECT_EQ(matrix_equal(m, copy), 1);
}

TEST(MatrixTest, MatrixMultiplyTest) {
    
    struct matrix m = matrix(2, 3);
    struct matrix n = matrix(3, 2);

    double val = 1.0;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            matrix_set(m, i, j, val);
            val += 1.0;
        }
    }

    val = 1.0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            matrix_set(n, i, j, val);
            val += 1.0;
        }
    }

    struct matrix c = matrix(2, 2);
    matrix_set(c, 0, 0, 22.0);
    matrix_set(c, 0, 1, 28.0);
    matrix_set(c, 1, 0, 49.0);
    matrix_set(c, 1, 1, 64.0);

    EXPECT_EQ(matrix_equal(matrix_multiply(m, n), c), 1);
    EXPECT_EQ(matrix_equal(matrix_multiply(m, c), n), 0);
}

TEST(MatrixTest, MatrixTupleMultiplyTest) {
    
    struct matrix m = matrix(4, 4);

    double data[] = {1.0f, 2.0f, 3.0f, 4.0f, 2.0f, 4.0f, 4.0f, 2.0f,
                     8.0f, 6.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

    m.matrix = data;

    struct tuple t = tuple(1.0, 2.0, 3.0, 1.0);

    struct tuple expect = tuple(18.0, 24.0, 33.0, 1.0);
    struct tuple result = matrix_transform(m, t);

    EXPECT_EQ(tuple_equal(result, expect), 1);
}

TEST(MatrixTest, MatrixIdentityTest) {
    
    struct matrix m = matrix_identity(4);
    struct matrix i = matrix(4, 4);

    double id[] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

    i.matrix = id;

    struct matrix a = matrix(4, 4);

    double data[] = {0.0f, 1.0f, 2.0f, 4.0f, 1.0f, 2.0f, 4.0f, 8.0f,
                     2.0f, 4.0f, 8.0f, 16.0f, 4.0f, 8.0f, 16.0f, 32.0f};
    a.matrix = data;

    EXPECT_EQ(matrix_equal(m, i), 1);
    EXPECT_EQ(matrix_equal(a, matrix_multiply(a, m)), 1);
}
