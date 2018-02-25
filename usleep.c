#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    
    useconds_t ut = 100;

    if (argc>1) {
        ut = atoi(argv[1]);
    }

    ut = (ut<=100)?100:ut;
    ut = (ut>1000000)?1000000:ut;
    usleep(ut);

    return 0;
}

