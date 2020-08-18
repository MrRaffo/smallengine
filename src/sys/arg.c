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
 * returns 0 if parameter not found
 */
int arg_check(const char *arg)
{
        int i;

        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], arg) == 0) {
                        return i;
                }
        }

        return 0;
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


