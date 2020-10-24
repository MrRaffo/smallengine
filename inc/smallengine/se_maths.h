#ifndef __SE_MATHS_H__
#define __SE_MATHS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define EPSILON 0.00001     // threshold for comparing double values

/** @struct tuple
 *  @brief A 4-dimension tuple, used to represent vectors and points
 *  @var tuple.x
 *  @var tuple.y
 *  @var tuple.z
 *  @var tuple.w
 *  Each member represents a component of the vector/point
 */
struct tuple {
        double x;
        double y;
        double z;
        double w;
};

/** @struct matrix
 *  @brief A 2-dimension matrix, mainly for transformation of vectors/points, row major
 *  @var matrix.row
 *  the number of rows in the matrix
 *  @var matrix.col
 *  the number of rows in the matrix
 *  @var matrix
 *  A pointer of type double, the data in the matrix
 */
struct matrix {
        int row;
        int col;
        double *matrix;
};

/* for convenience */
typedef struct tuple point;
typedef struct tuple vector;

/*****************
 * MISC MATHS
 ****************/

/*
 * return 1 if the two values passed differ by less than EPSILON, otherwise
 * return 0
 */
const int double_equal(const double val1, const double val2);

/* 
 * compare two doubles, return 0 if they are equal, -1 if the first value is
 * less than the second and 1 if it is larger
 */
const int double_compare(const double val1, const double val2);

/******************
 * TUPLE
 *****************/

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

/*********************
 * MATRIX
 ********************/

/* CREATION */

/* Return a matrix, all elements initiated to 0.0f */
struct matrix matrix(const int r, const int c);

/* Returns the identity matrix for a given dimension */
struct matrix matrix_identity(const int size);

/* Returns 1 if matrix is the null matrix ({0, 0, NULL}), 0 if valid */
int matrix_is_null(const struct matrix m);

/* Return a matrix in a string */
char *matrix_to_string(const struct matrix m);

/* output matrix data array, mostly for debugging */
char *matrix_to_string_raw(const struct matrix m);

/* OPERATIONS */

/* return a copy of the given matrix */
struct matrix matrix_copy(const struct matrix m);

/* return a copy of the given matrix */
struct matrix matrix_copy(const struct matrix m);

/* Set the value of a row, col position, 0 if unable, 1 on success */
int matrix_set(struct matrix m, int r, int c, double value);

/* Retrieve a value from the matrix, returns 0.0f on bad index */
double matrix_get(const struct matrix m, int r, int c);

/* Check if two given matrices are equal, returns 1 if so, 0 otherwise */
int matrix_equal(const struct matrix m, const struct matrix n);

/* Multiply two matrices if possible, return a new matrix if successful,
 * returns a null matrix {0, 0, NULL} on failure */
struct matrix matrix_multiply(const struct matrix m, const struct matrix n);

/* Return the 4 element tuple resulting in multiplying the given matrix and
 * tuple, rejects any matrix with row != 4 */
const struct tuple matrix_transform(const struct matrix m, const struct tuple t);

/* takes a list of matrices and returns their product, list must be null 
 * terminated and at least one matrix must be provided. all transformations
 * will be applied to an identity matrix, so this need not be part of the
 * list. operations performed in reverse, ie, intuitive, order */
struct matrix transform(struct matrix *m, ...);

/* Return the transpose of the given matrix */
struct matrix matrix_transpose(const struct matrix m);

/* returns the determinant, matrix must be 2x2, returns 0 on fail */
double matrix_2determinant(const struct matrix m);

/* returns the determinant, matrix must be 3x3, returns 0 on fail */
double matrix_3determinant(const struct matrix m);

/* returns the minor of a 3x3 matrix at given point */
double matrix_minor(struct matrix m, int row, int col);

/* return the given matrix with the specified column and row removed
 * returns the null matrix on failure */
struct matrix submatrix(const struct matrix m, const int r, const int c);

/* return the cofactor of a given 3x3 matrix at the given element, 0.0f on fail */
double matrix_cofactor(const struct matrix m, const int r, const int c);

/* return the determinant of a matrix, 0.0f if unable for some reason */
double matrix_determinant(const struct matrix m);

/* test if a matrix is invertible, returns 0 if not, 1 if it is *
 * (a matrix is invertible if its determinant is non-zero
 */
const int matrix_invertible(const struct matrix m);

/* return the inverse of a matrix if it exists, returns null matrix on fail */
struct matrix matrix_inverse(const struct matrix m);

/* return a matrix that will translate by x, y, z */
struct matrix matrix_translate(const double x, const double y, const double z);

/* return a matrix to scale a tuple */
struct matrix matrix_scale(const double x, const double y, const double z);

/* return a matrix that will rotate around the x-axis, radians */
struct matrix matrix_rotate_x(const double radians);

/* return a matrix that will rotate around the y-axis, radians */
struct matrix matrix_rotate_y(const double radians);

/* return a matrix that will rotate around the z-axis, radians */
struct matrix matrix_rotate_z(const double radians);

/* return a matrix that shears */
struct matrix matrix_shear(const double xy, const double xz, const double yx, const double yz, const double zx, const double zy);

/* view transform *
 * create a matrix that transforms the world relative the 'eye' position */
struct matrix matrix_view_transform(struct tuple from, struct tuple to, struct tuple up);




#ifdef __cplusplus
}
#endif

#endif // header guard
