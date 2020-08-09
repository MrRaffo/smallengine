#include <stdio.h>
#include <assert.h>
#include <math.h>

#include <smallengine/se_maths.h>
#include <smallengine/maths/tuple.h>

/* test the various creation functions for tuples, points and vectors */
void TST_TupleCreate()
{
        struct tuple t = tuple(1.0, 2.0, 3.0, 4.0);
        assert(double_equal(t.x, 1.0) == 1);
        assert(double_equal(t.y, 2.0) == 1);
        assert(double_equal(t.z, 3.0) == 1);
        assert(double_equal(t.w, 4.0) == 1);

        point p = point_3d(2.5, 3.5, 4.5);
        assert(double_equal(p.z, 4.5) == 1);
        assert(double_equal(p.w, 1.0) == 1); 
        
        vector v = vector_3d(2.5, 2.5, 1.5);
        assert(double_equal(v.y, 2.5) == 1);
        assert(double_equal(v.w, 0.0) == 1);

        point p2d = point_2d(0.1, 0.1);
        assert(double_equal(p2d.z, 0.0) == 1);
        assert(double_equal(p2d.x, 0.1) == 1);

        vector v2d = vector_2d(0.003, 0.5); 
        assert(double_equal(v2d.y, 0.5) == 1);
        
        printf("[Tuple Create] Complete, all tests pass!\n");
}

void TST_TupleEqual()
{
        struct tuple t1 = tuple(0.0000001, 0.5, 0.3, 2.5);
        struct tuple t2 = tuple(0.0000002, 0.4999999999, 0.3, 2.5);
        assert(tuple_equal(t1, t2) == 1);

        point p1 = point_3d(1.0, 1.0, 1.0);
        point p2 = point_3d(1.0, 1.0, 1.0);
        assert(tuple_equal(p1, p2) == 1);

        vector v1 = vector_2d(0.3, 0.4);
        vector v2 = vector_2d(0.2999999, 0.4);
        assert(tuple_equal(v1, v2) == 1);

        vector v3 = vector_2d(0.1, 0.1);
        vector v4 = vector_2d(90.0, 100.0);
        assert(tuple_equal(v3, v4) == 0);

        printf("[Tuple Equal] Complete, all tests pass!\n");
}

void TST_TupleAdd()
{
        struct tuple t1 = tuple(1.0, 1.0, 1.0, 1.0);
        struct tuple t2 = tuple(0.5, 1.5, 2.5, 3.5);
        struct tuple t3 = tuple(1.5, 2.5, 3.5, 4.5);
        struct tuple t4 = tuple_add(t1, t2);
        
        assert(tuple_equal(t3, t4) == 1);

        point p = point_3d(1.0, 0.49999999, 0.0);
        vector v = vector_3d(1.0, 0.5, 0.0);
        point r = point_3d(2.0, 1.0, 0.0);
        point r2 = tuple_add(p, v);

        assert(tuple_equal(r, r2) == 1);

        printf("[Tuple Add] Complete, all tests pass!\n");
}

void TST_TupleSubtract()
{
        struct tuple t1 = tuple(1.0, 1.0, 1.0, 1.0);
        struct tuple t2 = tuple(0.5, 1.5, 2.5, 3.5);
        struct tuple t3 = tuple(0.5, -0.5, -1.5, -2.5);
        struct tuple t4 = tuple_subtract(t1, t2);
        
        assert(tuple_equal(t3, t4) == 1);

        point p = point_3d(1.0, 0.49999999, 0.0);
        vector v = vector_3d(1.0, 0.5, 0.0);
        point r = point_3d(0.0, 0.0, 0.0);
        point r2 = tuple_subtract(p, v);

        assert(tuple_equal(r, r2) == 1);

        printf("[Tuple Subtract] Complete, all tests pass!\n");
}

void TST_TupleNegate()
{
        struct tuple t1 = tuple(0.0, 1.0, 2.0, 3.0);
        struct tuple t2 = tuple(0.0, -1.0, -2.0, -3.0);
        assert(tuple_equal(tuple_negate(t1), t2) == 1);

        printf("[Tuple Negate] Complete, all tests pass!\n");
}

void TST_TupleScale()
{
        struct tuple t1 = tuple(1.0, 1.0, 1.0, 1.0);
        struct tuple t2 = tuple(10.0, 10.0, 10.0, 10.0);
        assert(tuple_equal(tuple_scale(t1, 10.0), t2) == 1);

        printf("[Tuple Scale] Complete, all tests pass\n");
}

void TST_TupleDivide()
{
        struct tuple t1 = tuple(1.0, 1.0, 1.0, 1.0);
        struct tuple t2 = tuple(10.0, 10.0, 10.0, 10.0);
        assert(tuple_equal(tuple_divide(t2, 10.0), t1) == 1);

        printf("[Tuple Divide] Complete, all tests pass!\n");
}

void TST_VectorMagnitude()
{
        vector v1 = vector_3d(1.0, 2.0, 3.0);
        vector v2 = vector_3d(3.0, 4.0, 5.0);

        assert(double_equal(vector_magnitude(v1), sqrt(14.0)) == 1);
        assert(double_equal(vector_magnitude(v2), sqrt(50.0)) == 1);

        printf("[Vector Magnitude] Complete, all tests pass!\n");
}

void TST_VectorCompare()
{
        vector v1 = vector_3d(1.0, 2.0, 3.0);
        vector v2 = vector_3d(3.0, 4.0, 5.0);
        vector v3 = vector_3d(-1.0, -2.0, -3.0);
        vector v4 = vector_3d(-10.0, -10.0, -10.0);

        assert(vector_compare(v1, v2) == -1);
        assert(vector_compare(v2, v1) == 1);
        assert(vector_compare(v1, v3) == 0);
        assert(vector_compare(v4, v1) == 1);

        printf("[Vector Compare] Complete, all tests pass!\n");
}

void TST_VectorMagnitudeCompare()
{
        vector v = vector_3d(1.0, 2.0, 3.0);
        double m = sqrt(14.0);

        assert(vector_magnitude_compare(v, m) == 0);
        assert(vector_magnitude_compare(v, 1.0) == 1);
        assert(vector_magnitude_compare(v, 100.0) == -1);

        printf("[Vector Magnitude Compare] Complete, all tests pass!\n");
}

void TST_VectorNormal()
{
        struct tuple v1 = vector_3d(1.5f, 5.0f, 0.0f);
        struct tuple v2 = vector_3d(0.2f, 1.8f, 6.2f);
        struct tuple v3 = vector_3d(-8.65f, -6.234f, 1.0f);
        struct tuple v4 = vector_3d(2.0f, 3.0f, 4.0f);
        struct tuple v5 = vector_3d(1.0f, 2.0f, 3.0f);
        struct tuple v6 = vector_3d(45.3f, -90.123f, 3.3f);
        struct tuple v7 = vector_3d(0.0f, 0.0f, 0.0f);       // special case

        assert(double_equal(vector_magnitude(vector_normal(v1)), 1.0f) == 1);
        assert(double_equal(vector_magnitude(vector_normal(v2)), 1.0f) == 1);
        assert(double_equal(vector_magnitude(vector_normal(v3)), 1.0f) == 1);
        assert(double_equal(vector_magnitude(vector_normal(v4)), 1.0f) == 1);
        assert(double_equal(vector_magnitude(vector_normal(v5)), 1.0f) == 1);
        assert(double_equal(vector_magnitude(vector_normal(v6)), 1.0f) == 1);
        vector_magnitude(vector_normal(v7));    // just don't break...
        
        printf("[Vector Normal] Complete, all tests pass!\n");
}

void TST_VectorDot()
{
        struct tuple v1 = vector_3d(1.0f, 2.0f, 3.0f);
        struct tuple v2 = vector_3d(2.0f, 3.0f, 4.0f);
        struct tuple v3 = vector_3d(3.0f, -3.0f, 3.0f);

        assert(double_equal(vector_dot(v1, v2), 20.0f) == 1);
        assert(double_equal(vector_dot(v1, v3), 6.0f) == 1);
        assert(double_equal(vector_dot(v2, v3), 9.0f) == 1);
        assert(double_equal(vector_dot(v3, v3), 27.0f) == 1);

        printf("[Vector Dot] Complete, all tests pass!\n");
}

void TST_VectorCross()
{
        struct tuple a = vector_3d(1.0f, 0.0f, 0.0f);
        struct tuple b = vector_3d(0.0f, 1.0f, 0.0f);
        struct tuple c = vector_3d(0.0f, 0.0f, 1.0f);

        assert(tuple_equal(vector_cross(a, b), c) == 1);
        assert(tuple_equal(vector_cross(b, a), tuple_negate(c)) == 1);
        assert(tuple_equal(vector_cross(c, a), b) == 1);
        assert(tuple_equal(vector_cross(b, c), a) == 1);

        printf("[Vector Cross] Complete, all tests pass!\n");
}

// test reflection of vector against given surface normal
void TST_VectorReflect()
{
        struct tuple vec = vector_3d(1.0, -1.0, 0.0);
        struct tuple nor = vector_3d(0.0, 1.0, 0.0);
        struct tuple ref = vector_reflect(vec, nor);

        assert(tuple_equal(ref, vector_3d(1.0, 1.0, 0.0)) == 1);

        // reflect vertical drop against 45 degree incline should produce
        // horizontal vector
        vec = vector_3d(0.0, -1.0, 0.0);
        nor = vector_3d(sqrt(2.0)/2.0, sqrt(2.0)/2.0, 0.0);
        ref = vector_reflect(vec, nor);

        assert(tuple_equal(ref, vector_3d(1.0, 0.0, 0.0)) == 1);

        printf("[Vector Reflect] Complete, all tests pass!\n");
}

int main()
{
        TST_TupleCreate();
        TST_TupleEqual();
        TST_TupleAdd();
        TST_TupleSubtract();
        TST_TupleNegate();
        TST_TupleScale();
        TST_TupleDivide();
        TST_VectorMagnitude();
        TST_VectorCompare();
        TST_VectorMagnitudeCompare();
        TST_VectorNormal();
        TST_VectorDot();
        TST_VectorCross();
        TST_VectorReflect();

        return 0;
}
