#include <stdio.h>
#include <stdlib.h>
#include "util.h"

void print_error_code(int error)
{
    char buffer[32];

    sprintf(buffer, "%d", error);
    print_error_msg(buffer);
}

void print_error_msg(const char *msg)
{
    printf("ERROR: %s\n", msg);
}

void print_msg(const char *msg)
{
    printf("%s\n", msg);
}
