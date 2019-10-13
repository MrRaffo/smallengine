#ifndef __tuple_h__
#define __tuple_h__

/*
 * contains all code related to the tuple data structure, used as vectors and
 * points in geometry.
 */

struct tuple {
        double x;
        double y;
        double z;
        double w;
};

/* for convenience */
typedef struct tuple point;
typedef struct tuple vector;

/* Creation and initialization */

/* create a new tuple with given values */
const struct tuple tuple(double x, double y, double z, double w);

/* returns a zero tuple ({0.0, 0.0, 0.0, 0.0}) */
const struct tuple tuple_zero();

/* returns a point at the origin ({0.0, 0.0, 0.0, 1.0}) */
const struct tuple point_origin();

/* create a point (w = 1.0) */
const struct tuple point_3d(double x, double y, double z);

/* create a point in 2d (z = 0.0, w = 1.0) */
const struct tuple point_2d(double x, double y);

/* create a vector (w = 0.0) */
const struct tuple vector_3d(double x, double y, double z);

/* create a vector in 2d (z = 0.0, w = 0.0) */
const struct tuple vector_2d(double x, double y);

/* Comparison */

/* return 1 if the tuples passed are the same */
const int tuple_equal(struct tuple x, struct tuple y);

/* Operations */

/* add two tuples togethe component by componet and return the result */
const struct tuple tuple_add(const struct tuple t1, const struct tuple t2);

/* subtract t2 from t1 and return the resulting tuple */
const struct tuple tuple_subtract(const struct tuple t1, const struct tuple t2);

/* return the resulting tuple found by multiplying all of the components of the
 * given tuple by -1.0 */
const struct tuple tuple_negate(const struct tuple t);

/* return the tuple found by multiplying all components of the given tuple by
 * the given scalar value */
const struct tuple tuple_scale(const struct tuple t, const double factor);

/* return the tuple found by dividing all components of the given tuple by 
 * the given scalar value */
const struct tuple tuple_divide(const struct tuple t, const double scalar);

/* Vector Operations */

/* get the length, or magnitude, of a vector */
const double vector_magnitude(const struct tuple v);

/* compare 2 vectors, return -1 if vec1 has smaller magniture, 1 if it is
 * greater and 0 if they are equal, doesn't call sqrt like vector magnitude
 * does */
const int vector_compare(const struct tuple vec1, const struct tuple vec2);

/* compare vector length to given value, return 0 if it is equal, 1 if vector
 * is greater than and -1 if it is less than. As per vector_compare, this
 * skips the sqrt() call */
const int vector_magnitude_compare(const struct tuple v, const double m);

/* return the normal vector in the direction the given vector points */
const struct tuple vector_normal(const struct tuple v);

/* return the dot product of two vectors (represents the angle between them */
const double vector_dot(const struct tuple v1, const struct tuple v2);

/* return the cross product - a new vector at right angles to the two passed */
const struct tuple vector_cross(const struct tuple v1, const struct tuple v2);

/* return a vector that has struct against the given normal vector */
const struct tuple vector_reflect(const struct tuple i, const struct tuple n);

#endif // __tuple_h__
