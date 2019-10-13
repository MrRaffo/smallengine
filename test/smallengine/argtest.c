#include <stdio.h>
#include <smallengine/sys/arg.h>

int main(int argc, char **argv)
{
        arg_init(argc, argv);

        printf("%d arguments passed:\n", arg_number());
        int i;
        for (i = 0; i < arg_number(); i++) {
                printf("\t%d: %s\n", i, arg_get(i));
        }

        if (arg_check("-test")) {
                printf("Argument '-test' was passed (%d)\n", 
                        arg_check("-test"));
        } else {
                printf("Argument '-test' was not passed\n");
        }

        return 0;
}
