#include <stdio.h>
#include <string.h>

#include <smallengine/se_sys.h>

/*
 * arg
 * handles all command line argument checking code
 */

static int argc = 0;
static char **argv = NULL;

/*
 * pass the programs argument count and pointer to the arg module
 */
void arg_init(int arg_count, char **arg_ptr)
{
        argc = arg_count;
        argv = arg_ptr;
}

/*
 * get number of arguments
 */
int arg_number(void)
{
        return argc;
}

/*
 * check if a parameter exists and give its index in the list
 * returns -1 if parameter not found
 */
int arg_check(const char *arg)
{
        int i;

        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], arg) == 0) {
                        return i;
                }
        }

        return -1;
}

/* @brief Get the value associated with a command line parameter
 *
 * Checks for a matching command line argument and returns a pointer to the next
 * argument in the list, if it exists, NULL otherwise
 *
 * @param arg the value to check for
 */
char *arg_get_value(const char *arg)
{
        int i = arg_check(arg) + 1;
        if (i > 0 && i < argc) {
                return argv[i];
        }

        return NULL;
}

/*
 * return a pointer to the argument at the given index, NULL on fail
 */
char *arg_get(int i)
{
        if (i >= 0 && i < argc) {
                return argv[i];
        }

        return NULL;
}


