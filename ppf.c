#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef int (*i2func)(int, int);

int add(int a, int b){
    return a+b;
}

int mul(int a, int b) {
    return a*b;
}

int main(int argc, char* argv[]) {
    
    i2func x = add;
    i2func y = mul;

    printf("%d , %d\n",x(2,3),y(4,6));

    return 0;
}

