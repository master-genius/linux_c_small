#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//void qsort(void *d, unsigned int size, int (*call)(void * a, void *b));

void qsorti(int *d, int start, int end) {
    if (start >= end) {
        return ;
    }

    int med = (end+start)/2;
    int center = d[med];
    int i = start;
    int j = end;
    int tmp = 0;

    while(1) {
        if (d[i]==d[j] && d[i]==center)break;
        while(d[i]<center && i<j )i++;
        while(d[j]>center && j>i)j--;
        if (i<j) {
            tmp = d[i];
            d[i] = d[j];
            d[j] = tmp;
        }
        else
            break;
    }
    if (i<med && i!=j) {
        tmp = d[i];
        d[i] = d[med];
        d[med] = tmp;
    }
    qsorti(d, start, i-1);
    qsorti(d, i+1,end);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        dprintf(2, "Error: enter some numbers\n");
        return -1;
    }

    int N = argc - 1;
    
    int *nms = (int*)malloc(sizeof(int)*N);
    if (nms==NULL) {
        perror("malloc");
        return -1;
    }

    for(int i=0;i<N;i++) {
        nms[i] = atoi(argv[i+1]);
    }

    qsorti(nms, 0, N-1);

    for (int i=0;i<N;i++) {
        printf("%d ", nms[i]);
    }
    printf("\n");

    free(nms);
    nms = NULL;

    return 0;
}

