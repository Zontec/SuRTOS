#include <stdio.h>
#include "product_specific.h"

int main(int argc, char **argv)
{
    char str1[20] = "hello";
    char str2[] = " world!";
    strcat(str1, str2);
    printf("%s\n", str1);

    return 1;
}