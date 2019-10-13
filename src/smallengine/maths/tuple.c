#include <stdio.h>
#include <math.h>

#include <smallengine/maths/maths.h>
#include <smallengine/maths/tuple.h>

/* Creation and initialization */

/* create a new tuple with given values */
const struct tuple tuple(double x, double y, double z, double w)
{
        struct tuple t = {x, y, z, w};
        return t;
}


/* returns a zero tuple ({0.0, 0.0, 0.0, 0.0}) */
const struct tuple tuple_zero()
{
        struct tuple t = {0.0, 0.0, 0.0, 0.0};
        return t;
}


/* returns a point at the origin ({0.0, 0.0, 0.0, 1.0}) */
const struct tuple point_origin()
{
        struct tuple p = {0.0, 0.0, 0.0, 1.0};
        return p;
}

/* create a point (w = 1.0) */
const struct tuple point_3d(double x, double y, double z)
{
        struct tuple p = {x, y, z, 1.0};
        return p;
}

/* create a point in 2d (z = 0.0, w = 1.0) */
const struct tuple point_2d(double x, double y)
{
        struct tuple p = {x, y, 0.0, 1.0};
        return p;
}

/* create a vector (w = 0.0) */
const struct tuple vector_3d(double x, double y, double z)
{
        struct tuple v = {x, y, z, 0.0};
        return v;
}

/* create a vector in 2d (z = 0.0, w = 0.0) */
const struct tuple vector_2d(double x, double y)
{
        struct tuple v = {x, y, 0.0, 0.0};
        return v;
}

/* Comparison */

/* return 1 if the tuples passed are the same */
const int tuple_equal(struct tuple t1, struct tuple t2)
{
        if (!double_equal(t1.x, t2.x)) { return 0; }
        if (!double_equal(t1.y, t2.y)) { return 0; }
        if (!double_equal(t1.z, t2.z)) { return 0; }
        if (!double_equal(t1.w, t2.w)) { return 0; }

        return 1;
}

/* Operations */

/* add two tuples togethe component by componet and return the result */
const struct tuple tuple_add(const struct tuple t1, const struct tuple t2)
{
        struct tuple t = {t1.x+t2.x, t1.y+t2.y, t1.z+t2.z, t1.w+t2.w};
        return t;
}

/* subtract t2 from t1 and return the resulting tuple */
const struct tuple tuple_subtract(const struct tuple t1, const struct tuple t2)
{
        struct tuple t = {t1.x-t2.x, t1.y-t2.y, t1.z-t2.z, t1.w-t2.w};
        return t;
}

/* return the resulting tuple found by multiplying all of the components of the
 * given tuple by -1.0 */
const struct tuple tuple_negate(const struct tuple t)
{
        struct tuple t1 = {t.x*-1.0, t.y*-1.0, t.z*-1.0, t.w*-1.0};
        return t1;
}

/* return the tuple found by multiplying all components of the given tuple by
 * the given scalar value */
const struct tuple tuple_scale(const struct tuple t, const double f)
{
        struct tuple t1 = {t.x * f, t.y * f, t.z * f, t.w * f};
        return t1;
}

/* return the tuple found by dividing all components of the given tuple by 
 * the given scalar value */
const struct tuple tuple_divide(const struct tuple t, const double s)
{
        struct tuple t1 = {t.x / s, t.y / s, t.z / s, t.w / s};
        return t1;
}

/* Vector Operations */

/* get the length, or magnitude, of a vector */
const double vector_magnitude(const struct tuple v)
{
        double mag = v.x * v.x + v.y * v.y + v.z * v.z;
        return sqrt(mag);
}

/* compare 2 vectors, return -1 if vec1 has smaller magniture, 1 if it is
 * greater and 0 if they are equal, doesn't call sqrt like vector magnitude
 * does */
const int vector_compare(const struct tuple vec1, const struct tuple vec2)
{
        double mag1 = vec1.x * vec1.x + vec1.y * vec1.y + vec1.z * vec1.z;
        double mag2 = vec2.x * vec2.x + vec2.y * vec2.y + vec2.z * vec2.z;

        if (double_equal(mag1, mag2)) {
                return 0;
        }

        return (mag1 > mag2) ? 1 : -1;
}

/* compare vector length to given value, return 0 if it is equal, 1 if vector
 * is greater than and -1 if it is less than. As per vector_compare, this
 * skips the sqrt() call */
const int vector_magnitude_compare(const struct tuple v, const double m)
{
        double vm = v.x * v.x + v.y * v.y + v.z * v.z;
        double mag = m * m;
        
        if (double_equal(vm, mag)) {
                return 0;
        }

        return (vm > mag) ? 1 : -1;
}

/* return the normal vector in the direction the given vector points */
const struct tuple vector_normal(const struct tuple v)
{
        double mag = vector_magnitude(v);
        if (mag == 0.0) return v;      // just in case...
        return vector_3d(v.x / mag, v.y / mag, v.z / mag);
}

/* return the dot product of two vectors (represents the angle between them */
const double vector_dot(const struct tuple v1, const struct tuple v2)
{
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/* return the cross product - a new vector at right angles to the two passed */
const struct tuple vector_cross(const struct tuple v1, const struct tuple v2)
{
        return vector_3d(v1.y * v2.z - v1.z * v2.y,
                         v1.z * v2.x - v1.x * v2.z,
                         v1.x * v2.y - v1.y * v2.x);
}

/* return a vector that has struct against the given normal vector */
const struct tuple vector_reflect(const struct tuple i, const struct tuple n)
{
        return tuple_subtract(i, tuple_scale(n, 2.0*vector_dot(i, n)));
}

