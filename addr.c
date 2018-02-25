#include <stdio.h>

int main(int argc, char* argv[]) {
    int a = 1;
    unsigned int ad = sizeof(&a);
    printf("%X -- %d\n",&a,ad);
    return 0;
}
