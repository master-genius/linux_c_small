#include <stdio.h>

void swap(int *a, int *b)
{
    *b ^= *a ^= *b ^= *a;
}

int
main (int argc, char *argv)
{
    int a=12, b=23;
    swap(&a, &b);
    printf("%d,%d\n",a,b);
    return 0;
}
