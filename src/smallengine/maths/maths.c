#include <math.h>
#include "smallengine/maths/maths.h"

/*
 * return 1 if the two values passed differ by less than EPSILON, otherwise
 * return 0
 */
const int double_equal(const double val1, const double val2)
{
        return (fabs(val1 - val2)) < EPSILON ? 1 : 0;
}


/* 
 * compare two doubles, return 0 if they are equal, -1 if the first value is
 * less than the second and 1 if it is larger
 */
const int double_compare(const double val1, const double val2)
{
        if (double_equal(val1, val2)) {
                return 0;
        }

        return (val1 - val2) > 0 ? 1 : -1;
}


