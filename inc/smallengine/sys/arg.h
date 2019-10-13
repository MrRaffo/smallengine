#ifndef __arg_h__
#define __arg_h__

/*
 * arg
 * handles all command line argument checking code
 */

/*
 * pass the programs argument count and pointer to the arg module
 */
void arg_init(int argc, char **argv);

/*
 * get number of arguments
 */
int arg_number(void);

/*
 * check if a parameter exists and give its index in the list
 */
int arg_check(char *arg);

/*
 * return a pointer to the argument at the given index, NULL on fail
 */
char *arg_get(int i);

#endif // __arg_h__
