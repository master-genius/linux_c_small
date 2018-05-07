#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BITS_LEN        73

void out_bits(char *bits)
{
    for (int i=0;i<BITS_LEN;i++) {
        printf("%d%c", bits[i],(((i+1)%15==0)?'\n':' '));
    }
}

int
main(int argc, char *argv[])
{
    char bits[BITS_LEN] = {0};

    for (int i=0;i<BITS_LEN;i++) {
        for (int j=0;j<=i;j++) {
            bits[j] = !bits[j];
            out_bits(bits);
            usleep(60000);
            printf("\x1b[2J\x1b[;H");
        }
    }

    return 0;
}
