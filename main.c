
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "main.h"

int main(int argc, char const *argv[])
{
    int first_val = 0;
    int second_val = 0;
    int result;

    printf("argc: %d, %s, %s, %s\n", argc, argv[0], argv[1], argv[2]);

    if(argc == 3)
    {
        first_val = isdigit(argv[1]);
        second_val = isdigit(argv[2]);
    }

    if (first_val != 0)
    {
        result = sum_two_num(first_val, second_val);
        print_val(ADD, result);

        result = sub_two_num(first_val, second_val);
        print_val(SUB, result);

        result = mul_two_num(first_val, second_val);
        print_val(MUL, result);

        result = dev_two_num(first_val, second_val);
        print_val(DEV, result);
    }

    if (argv[0] != 0)
    {
        print_str(argv[0]);
    }


    return 0;
}
