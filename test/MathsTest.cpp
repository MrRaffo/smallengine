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

TEST(MatrixTest, MatrixTransposeTest) {
    
    struct matrix m = matrix(4, 4);
    struct matrix n = matrix(4, 4);

    double data[] = {0.0f, 9.0f, 3.0f, 0.0f,
                     9.0f, 8.0f, 0.0f, 8.0f,
                     1.0f, 8.0f, 5.0f, 3.0f,
                     0.0f, 0.0f, 5.0f, 8.0f};

    double datb[] = {0.0f, 9.0f, 1.0f, 0.0f,
                     9.0f, 8.0f, 8.0f, 0.0f,
                     3.0f, 0.0f, 5.0f, 5.0f,
                     0.0f, 8.0f, 3.0f, 8.0f};

    m.matrix = data;
    n.matrix = datb;

    EXPECT_EQ(matrix_equal(matrix_transpose(m), n), 1);
    EXPECT_EQ(matrix_equal(matrix_transpose(matrix_identity(4)), matrix_identity(4)), 1);
    EXPECT_EQ(matrix_equal(matrix_transpose(matrix_identity(3)), matrix_identity(3)), 1);
    EXPECT_EQ(matrix_equal(matrix_transpose(matrix_identity(2)), matrix_identity(2)), 1);
}

TEST(MatrixTest, Matrix2DeterminantTest) {
    
    EXPECT_EQ(matrix_2determinant(matrix(3, 3)), 0.0);

    struct matrix m = matrix(2, 2);
    EXPECT_EQ(matrix_2determinant(m), 0.0);

    double data[] = {1.0, 5.0, -3.0, 2.0};
    m.matrix = data;

    EXPECT_EQ(double_equal(matrix_2determinant(m), 17.0), 1);
}

TEST(MatrixTest, SubMatrixTest) {

    struct matrix m3 = matrix(3, 3);
    double m3data[] = {1.0f, 5.0f, 0.0f,
                      -3.0f, 2.0f, 7.0f,
                       0.0f, 6.0f, -3.0f};

    struct matrix sm3 = matrix(2, 2);
    double sm3data[] = {-3.0f, 2.0f,
                         0.0f, 6.0f};

    struct matrix m4 = matrix(4, 4);
    double m4data[] = {-6.0f, 1.0f, 1.0f, 6.0f,
                       -8.0f, 5.0f, 8.0f, 6.0f,
                       -1.0f, 0.0f, 8.0f, 2.0f,
                       -7.0f, 1.0f, -1.0f, 1.0f};

    struct matrix sm4 = matrix(3, 3);
    double sm4data[] = {-6.0f, 1.0f, 6.0f,
                        -8.0f, 8.0f, 6.0f,
                        -7.0f, -1.0f, 1.0f};

    m3.matrix = m3data;
    sm3.matrix = sm3data;
    m4.matrix = m4data;
    sm4.matrix = sm4data;

    EXPECT_EQ(matrix_equal(submatrix(m3, 0, 2), sm3), 1);
    EXPECT_EQ(matrix_equal(submatrix(m4, 2, 1), sm4), 1);
}

TEST(MatrixTest, MatrixMinorTest) {

    struct matrix m = matrix(3, 3);
    double mdata[] = {1.0f, 5.0f, 0.0f,
                     -3.0f, 2.0f, 7.0f,
                      0.0f, 6.0f, -3.0f};
    m.matrix = mdata;

    struct matrix n = matrix(3, 3);
    double ndata[] = {-6.0f, 1.0f, 6.0f,
                      -8.0f, 8.0f, 6.0f,
                      -7.0f, -1.0f, 1.0f};
    n.matrix = ndata;

    EXPECT_EQ(double_equal(matrix_minor(m, 1, 2), 6.0), 1);
    EXPECT_EQ(double_equal(matrix_minor(m, 0, 0), -48.0), 1);
    EXPECT_EQ(double_equal(matrix_minor(n, 2, 1), 12.0), 1);
    EXPECT_EQ(double_equal(matrix_minor(n, 2, 2), -40.0), 1);
}

TEST(MatrixTest, MatrixCofactorTest) {
    
    struct matrix m = matrix(3, 3);
    double mdata[] = {3.0f, 5.0f, 0.0f,
                      2.0f, -1.0f, -7.0f,
                      6.0f, -1.0f, 5.0f};
    m.matrix = mdata;

    EXPECT_EQ(double_equal(matrix_minor(m, 0, 0), -12.0), 1);
    EXPECT_EQ(double_equal(matrix_cofactor(m, 0, 0), -12.0), 1);
    EXPECT_EQ(double_equal(matrix_cofactor(m, 1, 0), -25.0), 1);
}

TEST(MatrixTest, MatrixDeterminantTest) {

    struct matrix m3 = matrix(3, 3);
    double m3data[] = {1.0f, 2.0f, 6.0f,
                      -5.0f, 8.0f, -4.0f,
                       2.0f, 6.0f, 4.0f};
    m3.matrix = m3data;


    struct matrix m4 = matrix(4, 4);
    double m4data[] = {-2.0f, -8.0f, 3.0f, 5.0f,
                       -3.0f, 1.0f, 7.0f, 3.0f,
                        1.0f, 2.0f, -9.0f, 6.0f,
                       -6.0f, 7.0f, 7.0f, -9.0f};
    m4.matrix = m4data;

    EXPECT_EQ(double_equal(matrix_cofactor(m3, 0, 0), 56.0), 1);
    EXPECT_EQ(double_equal(matrix_cofactor(m3, 0, 1), 12.0), 1);
    EXPECT_EQ(double_equal(matrix_cofactor(m3, 0, 2), -46.0), 1);
    EXPECT_EQ(double_equal(matrix_determinant(m3), -196.0), 1);

    EXPECT_EQ(double_equal(matrix_cofactor(m4, 0, 0), 690.0), 1);
    EXPECT_EQ(double_equal(matrix_cofactor(m4, 0, 1), 447.0), 1);
    EXPECT_EQ(double_equal(matrix_cofactor(m4, 0, 2), 210.0), 1);
    EXPECT_EQ(double_equal(matrix_cofactor(m4, 0, 3), 51.0), 1);
    EXPECT_EQ(double_equal(matrix_determinant(m4), -4071.0), 1);
}

TEST(MatrixTest, MatrixInvertibleTest) {
    
    struct matrix invertible = matrix(4, 4);
    double minv[] = {6.0f, 4.0f, 4.0f, 4.0f,
                     5.0f, 5.0f, 7.0f, 6.0f,
                     4.0f, -9.0f, 3.0f, -7.0f,
                     9.0f, 1.0f, 7.0f, -6.0f};
    invertible.matrix = minv;

    struct matrix noninvertible = matrix(4, 4);
    double mnon[] = {-4.0f, 2.0f, -2.0f, -3.0f,
                      9.0f, 6.0f, 2.0f, 6.0f,
                      0.0f, -5.0f, 1.0f, -5.0f,
                      0.0f, 0.0f, 0.0f, 0.0f};
    noninvertible.matrix = mnon;

    EXPECT_EQ(matrix_invertible(invertible), 1);
    EXPECT_EQ(matrix_invertible(noninvertible), 0);
}

TEST(MatrixTest, MatrixInverseTest) {
         
    struct matrix m1 = matrix(4, 4);
    struct matrix m2 = matrix(4, 4);
    struct matrix m3 = matrix(4, 4);

    double m1data[] = {-5.0f, 2.0f, 6.0f, -8.0f,
                        1.0f, -5.0f, 1.0f, 8.0f,
                        7.0f, 7.0f, -6.0f, -7.0f,
                        1.0f, -3.0f, 7.0f, 4.0f};

    double m2data[] = {8.0f, -5.0f, 9.0f, 2.0f,
                       7.0f, 5.0f, 6.0f, 1.0f,
                      -6.0f, 0.0f, 9.0f, 6.0f,
                      -3.0f, 0.0f, -9.0f, -4.0f};

    double m3data[] = {9.0f, 3.0f, 0.0f, 9.0f,
                      -5.0f, -2.0f, -6.0f, -3.0f,
                      -4.0f, 9.0f, 6.0f, 4.0f,
                      -7.0f, 6.0f, 6.0f, 2.0f};

    struct matrix im1 = matrix(4, 4);
    struct matrix im2 = matrix(4, 4);
    struct matrix im3 = matrix(4, 4);

    double im1data[] = {0.21805f, 0.45113f, 0.24060f, -0.04511f,
                       -0.80827f, -1.45677f, -0.44361f, 0.52068f,
                       -0.07895f, -0.22368f, -0.05263f, 0.19737f,
                       -0.52256f, -0.81391f, -0.30075f, 0.30639f};

    double im2data[] = {-0.15385f, -0.15385f, -0.28205f, -0.53846f,
                        -0.07692f, 0.12308f, 0.02564f, 0.03077f,
                         0.35897f, 0.35897f, 0.43590f, 0.92308f,
                        -0.69231f, -0.69231f, -0.76923f, -1.92308f};

    double im3data[] = {-0.04074f, -0.07778f, 0.14444f, -0.22222f,
                        -0.07778f, 0.03333f, 0.36667f, -0.33333f,
                        -0.02901f, -0.14630f, -0.10926f, 0.12963f,
                         0.17778f, 0.06667f, -0.26667f, 0.33333f};

    m1.matrix = m1data;
    m2.matrix = m2data;
    m3.matrix = m3data;

    im1.matrix = im1data;
    im2.matrix = im2data;
    im3.matrix = im3data;
    
    EXPECT_EQ(matrix_equal(matrix_inverse(m1), im1), 1); 
    EXPECT_EQ(matrix_equal(matrix_inverse(m2), im2), 1); 
    EXPECT_EQ(matrix_equal(matrix_inverse(m3), im3), 1); 
}

TEST(MatrixTest, MatrixTranslateTest) {
    
        struct tuple point = point_3d(-3.0f, 4.0f, 5.0f);
        struct matrix transform = matrix_translate(5.0f, -3.0f, 2.0f);
        struct tuple tpoint = tuple(2.0f, 1.0f, 7.0f, 1.0f);
        
        EXPECT_EQ(tuple_equal(matrix_transform(transform, point), tpoint), 1);

        tpoint = tuple(-8.0f, 7.0f, 3.0f, 1.0f);
        EXPECT_EQ(tuple_equal(matrix_transform(matrix_inverse(transform), point), tpoint), 1);
        
        struct tuple vec =  vector_3d(1.0f, 2.0f, 3.0f);
        EXPECT_EQ(tuple_equal(matrix_transform(transform, vec), vec), 1);
}

TEST(MatrixTest, MatrixScaleTest) {

        struct matrix transform = matrix_scale(2.0f, 3.0f, 4.0f);
        struct tuple point = point_3d(-4.0f, 6.0f, 8.0f);
        struct tuple vec = vector_3d(-4.0f, 6.0f, 8.0f);

        struct tuple scaled_point = point_3d(-8.0f, 18.0f, 32.0f);
        struct tuple scaled_vec = vector_3d(-8.0f, 18.0f, 32.0f);

        EXPECT_EQ(tuple_equal(matrix_transform(transform, point), scaled_point), 1);
        EXPECT_EQ(tuple_equal(matrix_transform(transform, vec), scaled_vec), 1);

        transform = matrix_inverse(transform);
        scaled_vec = vector_3d(-2.0f, 2.0f, 2.0f);
        EXPECT_EQ(tuple_equal(matrix_transform(transform, vec), scaled_vec), 1);

        // reflection on x axis
        transform = matrix_scale(-1.0f, 1.0f, 1.0f);
        point = point_3d(2.0f, 3.0f, 4.0f);
        scaled_point = point_3d(-2.0f, 3.0f, 4.0f);
        EXPECT_EQ(tuple_equal(matrix_transform(transform, point), scaled_point), 1);
}

TEST(MatrixTest, MatrixRotateXTest) {

        struct tuple point = point_3d(0.0f, 1.0f, 0.0f);
        struct matrix rot45 = matrix_rotate_x(M_PI / 4.0f);
        struct matrix rot90 = matrix_rotate_x(M_PI / 2.0f);

        struct tuple point45 = point_3d(0.0f, (sqrt(2.0f)/2.0f),(sqrt(2.0f)/2.0f));
        struct tuple point90 = point_3d(0.0f, 0.0f, 1.0f);

        EXPECT_EQ(tuple_equal(matrix_transform(rot45, point), point45), 1);
        EXPECT_EQ(tuple_equal(matrix_transform(rot90, point), point90), 1);
}


TEST(MatrixTest, MatrixRotateYTest) {

        struct tuple point = point_3d(0.0f, 0.0f, 1.0f);
        struct matrix rot45 = matrix_rotate_y(M_PI / 4.0f);
        struct matrix rot90 = matrix_rotate_y(M_PI / 2.0f);

        struct tuple point45 = point_3d((sqrt(2.0f)/2.0f), 0.0f, (sqrt(2.0f)/2.0f));
        struct tuple point90 = point_3d(1.0f, 0.0f, 0.0f);

        EXPECT_EQ(tuple_equal(matrix_transform(rot45, point), point45), 1);
        EXPECT_EQ(tuple_equal(matrix_transform(rot90, point), point90), 1);
}

TEST(MatrixTest, MatrixRotateZTest) {

        struct tuple point = point_3d(0.0f, 1.0f, 0.0f);
        struct matrix rot45 = matrix_rotate_z(M_PI / 4.0f);
        struct matrix rot90 = matrix_rotate_z(M_PI / 2.0f);

        struct tuple point45 = point_3d(-1.0f * (sqrt(2.0f)/2.0f), (sqrt(2.0f)/2.0f), 0.0f);
        struct tuple point90 = point_3d(-1.0f, 0.0f, 0.0f);

        EXPECT_EQ(tuple_equal(matrix_transform(rot45, point), point45), 1);
        EXPECT_EQ(tuple_equal(matrix_transform(rot90, point), point90), 1);
}

TEST(MatrixTest, MatrixShearTest) {

        struct matrix t;
        struct tuple sp;
        struct tuple p = point_3d(2.0f, 3.0f, 4.0f);

        // shear x in proportion to z
        t = matrix_shear(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        sp = point_3d(6.0f, 3.0f, 4.0f);
        EXPECT_EQ(tuple_equal(matrix_transform(t, p), sp), 1);

        // shear y in proportion to x
        t = matrix_shear(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
        sp = point_3d(2.0f, 5.0f, 4.0f);
        EXPECT_EQ(tuple_equal(matrix_transform(t, p), sp), 1);

        // shear y in proportion to z
        t = matrix_shear(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        sp = point_3d(2.0f, 7.0f, 4.0f);
        EXPECT_EQ(tuple_equal(matrix_transform(t, p), sp), 1);
        
        // shear z in proportion to x
        t = matrix_shear(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
        sp = point_3d(2.0f, 3.0f, 6.0f);
        EXPECT_EQ(tuple_equal(matrix_transform(t, p), sp), 1);
        
        // shear z in proportion to y
        t = matrix_shear(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        sp = point_3d(2.0f, 3.0f, 7.0f);
        EXPECT_EQ(tuple_equal(matrix_transform(t, p), sp), 1);
}

TEST(MatrixTest, MatrixChainTest) {

        struct tuple p = point_3d(1.0f, 0.0f, 1.0f);
        struct matrix r_x = matrix_rotate_x(M_PI / 2.0f);
        struct matrix s = matrix_scale(5.0f, 5.0f, 5.0f);
        struct matrix t = matrix_translate(10.0f, 5.0f, 7.0f);

        struct tuple result = matrix_transform(r_x, p);
        EXPECT_EQ(tuple_equal(result, point_3d(1.0f, -1.0f, 0.0f)), 1);

        result = matrix_transform(s, result);
        EXPECT_EQ(tuple_equal(result, point_3d(5.0f, -5.0f, 0.0f)), 1);

        result = matrix_transform(t, result);
        EXPECT_EQ(tuple_equal(result, point_3d(15.0f, 0.0f, 7.0f)), 1);

        struct matrix m = matrix_multiply(t,
                                matrix_multiply(s,
                                matrix_multiply(r_x, matrix_identity(4))));

        result = matrix_transform(m, p);
        EXPECT_EQ(tuple_equal(result, point_3d(15.0, 0.0, 7.0)), 1);
        // do it in one function
        struct matrix all_in_one = transform(&r_x, &s, &t, NULL);
        EXPECT_EQ(matrix_equal(m, all_in_one), 1);
        result = matrix_transform(all_in_one, p);
        EXPECT_EQ(tuple_equal(result, point_3d(15.0, 0.0, 7.0)), 1);
}

TEST(MatrixTest, MatrixChainTest2) {

        struct matrix r_x = matrix_rotate_x(M_PI / 2.0f);
        struct matrix s = matrix_scale(5.0f, 5.0f, 5.0f);
        struct matrix t = matrix_translate(10.0f, 5.0f, 7.0f);

        struct matrix m;
        /*
        m = matrix_multiply(r_x, matrix_identity(4));
        m = matrix_multiply(s, m);
        m = matrix_multiply(t, m);
        */
        m = matrix_multiply(t, matrix_multiply(s, matrix_multiply(r_x, matrix_identity(4))));

        struct matrix n = transform(&r_x, &s, &t, NULL);
        EXPECT_EQ(matrix_equal(m, n), 1);
}

TEST(MatrixTest, MatrixViewTransformTest) {

        struct tuple from = point_3d(0.0, 0.0, 0.0);
        struct tuple to = point_3d(0.0, 0.0, -1.0);
        struct tuple up = vector_3d(0.0, 1.0, 0.0);

        struct matrix t = matrix_view_transform(from, to, up);
        EXPECT_EQ(matrix_equal(t, matrix_identity(4)), 1);

        from = point_3d(0.0, 0.0, 8.0);
        to = point_3d(0.0, 0.0, 0.0);
        // up remains the same

        t = matrix_view_transform(from, to, up);
        EXPECT_EQ(matrix_equal(t, matrix_translate(0.0, 0.0, -8.0)), 1);

        from = point_3d(1.0, 3.0, 2.0);
        to = point_3d(4.0, -2.0, 8.0);
        up = vector_3d(1.0, 1.0, 0.0);

        t = matrix_view_transform(from, to, up);
        double res_data[] = {-0.50709, 0.50709, 0.67612, -2.36643,
                              0.76772, 0.60609, 0.12122, -2.82843,
                              -0.35857, 0.59761, -0.71714, 0.0000,
                              0.00000, 0.00000, 0.00000, 1.0000};
                              
        struct matrix result = matrix(4, 4);
        result.matrix = res_data;

        EXPECT_EQ(matrix_equal(t, result), 1);
}
