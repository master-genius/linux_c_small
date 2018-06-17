#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//void qsort(void *d, unsigned int size, int (*call)(void * a, void *b));

#define SWAP(a,b)   tmp=a;a=b;b=tmp;

void qsortstr(char *d[], int start, int end) {
    if (start >= end) {
        return ;
    }

    int med = (end+start)/2;
    int i = start;
    int j = end;
    char *tmp = NULL;
    
    SWAP(d[med],d[start]);

    for(j=start+1;j<=end;j++) {
        if (strcmp(d[j],d[start]) < 0) {
            i++;
            if (i==j)continue;
            SWAP(d[i],d[j]);
        }
    }

    SWAP(d[i],d[start]);

    qsortstr(d, start, i-1);
    qsortstr(d, i+1,end);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        dprintf(2, "Error: enter some string\n");
        return -1;
    }

    int N = argc - 1;

    qsortstr(argv, 1, N);

    for (int i=1;i<=N;i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");

    return 0;
}

