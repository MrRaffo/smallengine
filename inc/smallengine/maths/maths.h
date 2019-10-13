#ifndef __maths_h__
#define __maths_h__

#define EPSILON 0.00001         // threshold for comparing double values

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

#endif // __maths_h__
