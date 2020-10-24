#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include <smallengine/se_maths.h>
#include <smallengine/se_sys.h>
#include <smallengine/se_log.h>

const struct matrix NULL_MATRIX = {0, 0, NULL};

/* returns 0 if row != col, 1 otherwise */
static int _is_square(const struct matrix m)
{
        return (m.row == m.col) ? 1 : 0;       
}

/* returns 0 if attempting to access bad element coords */
static int _valid_element(const struct matrix m, const int r, const int c)
{
        if (r < 0 || r >= m.row || c < 0 || c >= m.col) {
                return 0;
        }

        return 1;
}

/* Return a matrix, all elements initiated to 0.0f */
struct matrix matrix(const int r, const int c)
{
        double *ptr = (double *)mem_alloc(r * c * sizeof(double));
        
        // all elements should be 0.0f on creation
        for (int i = 0; i < r * c; i++) {
                *(ptr+i) = 0.0f;
        }
        
        struct matrix m = {r, c, ptr};

        return m;
}


/* Returns the identity matrix for a given dimension */
struct matrix matrix_identity(const int size)
{
        struct matrix m = matrix(size, size);
        for (int i = 0; i < size; i++) {
                matrix_set(m, i, i, 1.0f);
        }

        return m;
}


/* Returns 1 if matrix is the null matrix ({0, 0, NULL}), 0 if valid */
int matrix_is_null(const struct matrix m)
{
        if (m.row == 0 && m.col == 0 && m.matrix == NULL) return 1;
        return 0;
}

// all numbers formatted using %g, 11 char max
#define NUM_LEN 11
#define START "("
#define SEP ", "
#define END ")\n"

/* Return a matrix in a string */
char *matrix_to_string(const struct matrix m)
{

        // a line of the matrix will be returned as 
        // "(0000.000, 0000.000 ... )\n" for as many rows and cols as
        // needed, each line has opening and closing brackets (2), a comma and
        // space separator for each element in a row (2 * r - 1), and a newline
        int rowlength = 2 + 2 * (m.row - 1) + m.row * NUM_LEN + 1;
        // + 1 for '\0'
        int strlength = m.col * rowlength + 1;

        char *buf = (char *)mem_alloc(strlength);
        char *pos = buf;

        for (int i = 0; i < m.row * m.col; i++) {
                
                pos += sprintf(pos, "%s%g%s", 
                        (i % m.col == 0) ? START : "",  // start of a line
                        m.matrix[i], 
                        (i % m.col == m.col-1) ? END : SEP); // end of a line
        }

        *pos = '\0';    // make sure it's terminated
        *(buf+strlength-1) = '\0';      // I mean it...

        return buf;
}

/* output matrix data array, mostly for debugging */
char *matrix_to_string_raw(const struct matrix m)
{
        int mlen = m.row * m.col;
        char *buf = (char *)mem_alloc(1 + mlen * NUM_LEN + 3);
        char *ptr = buf;

        ptr += sprintf(ptr, "(");
        for (int i = 0; i < m.row * m.col - 1; i++) {
                ptr += sprintf(ptr, "%g, ", *(m.matrix+i));
        }

        ptr += sprintf(ptr, "%g)\n", *(m.matrix+m.row*m.col-1));
        *ptr = '\0';
        *(buf+mlen-1) = '\0';
        return buf;
}

/* OPERATIONS */

static int _check_valid_element(const struct matrix m, int r, int c)
{
        if (r < 0 || r >= m.row || c < 0 || c >= m.col) {
                return 0;
        }

        return 1;
}

struct matrix matrix_copy(const struct matrix m)
{
        struct matrix copy = matrix(m.row, m.col);
        for (int i = 0; i < m.row * m.col; i++) {
                copy.matrix[i] = m.matrix[i];
        }

        return copy;
}

/* Set the value of a row, col position, 0 if unable, 1 on success */
int matrix_set(struct matrix m, int r, int c, double value)
{
        if (!_check_valid_element(m, r, c)) {
                log_err("Bad matrix index! (%d, %d) requested in (%d, %d) matrix.",
                        r, c, m.row, m.col);
                return 0;
        }
        
        *(m.matrix+(r*m.col+c)) = value;
        return 1;
}

/* Retrieve a value from the matrix, returns 0.0f on bad index */
double matrix_get(const struct matrix m, int r, int c)
{
        if (!_check_valid_element(m, r, c)) {
                log_err("Bad matrix index! (%d, %d) requested in (%d, %d) matrix.",
                        r, c, m.row, m.col);
                return 0.0f;
        }

        return *(m.matrix+(r*m.col+c));
}

/* Check if two given matrices are equal, returns 1 if so, 0 otherwise */
int matrix_equal(const struct matrix m, const struct matrix n)
{
        if (m.row != n.row || m.col != n.col) {
                return 0;
        }
        
        for (int i = 0; i < m.row * m.col; i++) {
                if (!double_equal(*(m.matrix+i), *(n.matrix+i))) {
                        return 0;
                }
        }

        return 1;
}

/* Multiply two matrices if possible, return a new matrix if successful,
 * returns a null matrix {0, 0, NULL} on failure */
struct matrix matrix_multiply(const struct matrix m, const struct matrix n)
{
        // check if compatible
        if (m.col != n.row) {
                return NULL_MATRIX;
        }

        struct matrix result = matrix(m.row, n.col);

        for (int r = 0; r < m.row; r++) {
                for (int c = 0; c < n.col; c++) {
                        double value = 0.0f;
                        for (int i = 0; i < m.col; i++) {
                                value += matrix_get(m, r, i) * matrix_get(n, i, c);
                        }
                        matrix_set(result, r, c, value);
                }
        }

        return result;
}

/* Return the 4 element tuple resulting in multiplying the given matrix and
 * tuple, rejects any matrix with row != 4 */
const struct tuple matrix_transform(const struct matrix m, const struct tuple t)
{
        if (m.row != 4) {
                log_err("Invalid matrix passed! (%d x %d)\n", m.row, m.col);
                return tuple(0.0f, 0.0f, 0.0f, 0.0f);
        }

        double values[4];
        for (int i = 0; i < 4; i++) {
                values[i] = matrix_get(m, i, 0) * t.x + 
                            matrix_get(m, i, 1) * t.y +
                            matrix_get(m, i, 2) * t.z +
                            matrix_get(m, i, 3) * t.w;
        }

        return tuple(values[0], values[1], values[2], values[3]);
}

/* takes a list of matrices and returns their product, list must be null 
 * terminated and at least one matrix must be provided. all transformations
 * will be applied to an identity matrix, so this need not be part of the
 * list. operations performed in reverse, ie, intuitive, order */
struct matrix transform(struct matrix *mat, ...)
{
        va_list argp;
        va_start(argp, mat);

        struct matrix transform = matrix_multiply(*mat, matrix_identity(4));
        struct matrix *ptr;
        while((ptr = (struct matrix *)va_arg(argp, void*)) != NULL) {
                transform = matrix_multiply(*ptr, transform);
        }

        va_end(argp);

        return transform;
}

/* Return the transpose of the given matrix */
struct matrix matrix_transpose(const struct matrix m)
{
        if (m.row != m.col) {
                log_err("Cannot transpose non square matrix (%d, %d)\n", m.row, m.col);
                return NULL_MATRIX;
        }

        struct matrix transpose = matrix(m.row, m.row);

        // for each index of a given matrix, it's new index is given by:
        // row_index * row_length + column_index
        for (int i = 0; i < m.row * m.col; i++) {
                *(transpose.matrix + (i % m.row) * m.row + (i / m.row)) = *(m.matrix+i);
        }

        return transpose;
}

/* returns the determinant, matrix must be 2x2 */
double matrix_2determinant(const struct matrix m)
{
        if (m.row != 2 || m.col != 2) {
                log_err("Unable to calculate determinant, matrix is (%d, %d)\n", m.row, m.col);
                return 0.0f;
        }

        return (m.matrix[0]*m.matrix[3]) - (m.matrix[1]*m.matrix[2]);
}

/* returns the determinant, matrix must be 3x3, returns 0 on fail */
double matrix_3determinant(const struct matrix m)
{
        if (m.row != 3 || m.col != 3) {
                log_err("Unable to calculate determinant, matrix is (%d, %d)\n", m.row, m.col);
                return 0.0f;
        }

        double *p = m.matrix;  // for brevity below

        return p[0]*p[4]*p[8] + p[1]*p[5]*p[6] + p[2]*p[3]*p[7] -
               p[2]*p[4]*p[6] - p[1]*p[3]*p[8] - p[0]*p[5]*p[7];
}

/* returns the minor of a 3x3 matrix at a given point */
double matrix_minor(struct matrix m, int row, int col)
{
        if (!_is_square(m) || !_valid_element(m, row, col)) {
                log_err("Unable to calculate minor, matrix (%d, %d)\n", m.row, m.col);
                return 0.0f;
        }

        if (m.row == 4) {
                return matrix_3determinant(submatrix(m, row, col));
        }

        // assume 3x3
        return matrix_2determinant(submatrix(m, row, col));
}

/* return the given matrix with the specified column and row removed */
struct matrix submatrix(const struct matrix m, const int r, const int c)
{
        if ((!_is_square(m) && m.row <= 1) || !_valid_element(m, r, c)) {
                log_err("Invalid request: m = %dx%d, (%d, %d)\n", m.row, m.col, r, c);
                return NULL_MATRIX;
        }

        int new_size = m.row - 1;

        // buffer for new matrix data
        double *data = (double *)mem_alloc(new_size*new_size*sizeof(double));
        int i, j;
        for (i = 0, j = 0; i < new_size * new_size; i++, j++) {
                while (j / m.row == r || j % m.row == c) {
                        j++;
                }

                data[i] = m.matrix[j];
        }

        struct matrix sub = {new_size, new_size, data};
        return sub;
}

/* return the cofactor of a given 3x3 matrix at the given element, 0.0f on fail */
double matrix_cofactor(const struct matrix m, const int r, const int c)
{
        if(!_is_square(m) || !_valid_element(m, r, c)) {
                log_err("Invalid request: m = %dx%d, (%d, %d)\n", m.row, m.col, r, c);
                return 0.0f;
        }

        double factor = ((r + c) & 1) ? -1.0f : 1.0f;
        return factor * matrix_minor(m, r, c);
}

/* return the determinant of a matrix, 0.0f if unable for some reason */
double matrix_determinant(const struct matrix m)
{
        if (!_is_square(m)) {
                log_err("Invalid request: m = %d x %d\n", m.row, m.col);
                return 0.0f;
        }

        if (m.row == 2) return matrix_2determinant(m);

        double determinant = 0.0f;
        for (int i = 0; i < m.row; i++) {
                determinant += m.matrix[i] * matrix_cofactor(m, 0, i);
        }
        
        return determinant;
}

/* test if a matrix is invertible, returns 0 if not, 1 if it is *
 * (a matrix is invertible if its determinant is non-zero
 */
const int matrix_invertible(const struct matrix m)
{
        return (matrix_determinant(m) == 0) ? 0 : 1;
}


/* return the inverse of a matrix if it exists, returns null matrix on fail */
struct matrix matrix_inverse(const struct matrix m)
{
        if (!matrix_invertible(m)) {
                log_err("Matrix not invertible\n");
                return NULL_MATRIX;
        }

        struct matrix inverse = matrix(m.row, m.col);
        double determinant = matrix_determinant(m);
        for (int r = 0; r < inverse.row; r++) {
                for (int c = 0; c < inverse.col; c++) {
                        double cofactor = matrix_cofactor(m, r, c);
                        matrix_set(inverse, c, r, cofactor / determinant);
                }
        }

        return inverse;
}

/* return a matrix that translates a point by x, y, z */
struct matrix matrix_translate(const double x, const double y, const double z)
{
        struct matrix m = matrix_identity(4); 

        m.matrix[3] = x;
        m.matrix[7] = y;
        m.matrix[11] = z;
        return m;
}

/* return a matrix to scale a tuple */
struct matrix matrix_scale(const double x, const double y, const double z)
{
        struct matrix scale = matrix_identity(4);
        scale.matrix[0] = x;
        scale.matrix[5] = y;
        scale.matrix[10] = z;
        return scale;
}

/* return a matrix that will rotate around the x-axis, radians */
struct matrix matrix_rotate_x(const double radians)
{
        struct matrix rot = matrix_identity(4);
        rot.matrix[5] = cos(radians);
        rot.matrix[6] = sin(radians) * -1.0f;
        rot.matrix[9] = sin(radians);
        rot.matrix[10] = cos(radians);
        return rot;
}

/* return a matrix that will rotate around the x-axis, radians */
struct matrix matrix_rotate_y(const double radians)
{
        struct matrix rot = matrix_identity(4);
        rot.matrix[0] = cos(radians);
        rot.matrix[2] = sin(radians);
        rot.matrix[8] = sin(radians) * -1.0f;
        rot.matrix[10] = cos(radians);
        return rot;
}

/* return a matrix that will rotate around the x-axis, radians */
struct matrix matrix_rotate_z(const double radians)
{
        struct matrix rot = matrix_identity(4);
        rot.matrix[0] = cos(radians);
        rot.matrix[1] = sin(radians) * -1.0f;
        rot.matrix[4] = sin(radians);
        rot.matrix[5] = cos(radians);
        return rot;
}

/* return a matrix that shears */
struct matrix matrix_shear(const double xy, const double xz, const double yx, const double yz, const double zx, const double zy)
{
        struct matrix shear = matrix_identity(4);
        shear.matrix[1] = xy;
        shear.matrix[2] = xz;
        shear.matrix[4] = yx;
        shear.matrix[6] = yz;
        shear.matrix[8] = zx;
        shear.matrix[9] = zy;
        return shear;
}

/* view transform *
 * create a matrix that transforms the world relative the 'eye' position */
struct matrix matrix_view_transform(struct tuple from, struct tuple to, struct tuple up)
{
        struct tuple forward = vector_normal(tuple_subtract(to, from));
        struct tuple upn = vector_normal(up);
        struct tuple left = vector_cross(forward, upn);
        struct tuple true_up = vector_cross(left, forward);

        struct matrix orientation = matrix(4, 4);
        
        orientation.matrix[0] = left.x;
        orientation.matrix[1] = left.y;
        orientation.matrix[2] = left.z;
        
        orientation.matrix[4] = true_up.x;
        orientation.matrix[5] = true_up.y;
        orientation.matrix[6] = true_up.z;

        orientation.matrix[8] = -forward.x;
        orientation.matrix[9] = -forward.y;
        orientation.matrix[10] = -forward.z;

        orientation.matrix[15] = 1.0;

        return matrix_multiply(orientation, matrix_translate(-from.x, -from.y, -from.z));
}
