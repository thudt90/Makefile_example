
#include <stdio.h>
#include "print_data.h"

int print_val(int val, int result)
{
    if (val == ADD)
    {
        printf("ADD two num is: %d\n", result);
    }
    else if (val == SUB)
    {
        printf("SUB two num is: %d\n", result);
    }
    else if (val == MUL)
    {
        printf("MUL two num is: %d\n", result);
    }
    else
    {
        printf("DEV two num is: %d\n", result);
    }

}

void print_str(const char * str)
{
    printf("CMD to run %s\n", str);
}

