#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>


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

void insert_sort(int *d, int n) {
    int tmp = 0;
    int j,k;
    for(k=1;k<n;k++){
        tmp = d[k];
        for(j=k;j>0 && d[j-1]>tmp;j--)
            d[j] = d[j-1];
        d[j] = tmp;
    }
}

#define ARGS_OUT        0
#define ARGS_SORT       1
#define ARGS_TIME       2

#define ARGS_END        3

#define SORT_INSERT     'i'
#define SORT_QUICK      'q'

int main(int argc, char *argv[])
{
    int N = 10000;
    char ARGS[ARGS_END] = {0,};
    ARGS[ARGS_OUT] = 1;
    ARGS[ARGS_SORT] = SORT_QUICK;

    if (argc>1) {
        for(int i=1;i<argc;i++) {
            if (strncmp(argv[i],"--sort=",7)==0) {
                if (strcmp(argv[i]+7, "insert")==0) {
                    ARGS[ARGS_SORT] = SORT_INSERT;
                } else if(strcmp(argv[i]+7,"qucik")==0) {
                    ARGS[ARGS_SORT] = SORT_QUICK;
                }
            }
            else if (strcmp(argv[i],"--no-out")==0){
                ARGS[ARGS_OUT] = 0;
            }
            else if (strcmp(argv[i],"--time")==0) {
                ARGS[ARGS_TIME] = 1;
            }
            else {
                N = atoi(argv[i]);
                N = (N<10)?10000:N;
            }
        }
    }
    
    int *nms = (int*)malloc(sizeof(int)*N);
    if (nms==NULL) {
        perror("malloc");
        return -1;
    }

    srand(time(NULL));
    for(int i=0;i<N;i++) {
        nms[i] = rand();
    }
    struct timeval  start_time,end_time;
    if (ARGS[ARGS_TIME])
        gettimeofday(&start_time,0);

    if (ARGS[ARGS_SORT] == SORT_QUICK) {
        qsorti(nms, 0, N-1);
    } else {
        insert_sort(nms, N);
    }

    if (ARGS[ARGS_TIME]) {
        gettimeofday(&end_time,0);
        double run_sec = (double)(end_time.tv_sec - start_time.tv_sec);
        double run_usec = ((double)(end_time.tv_usec - start_time.tv_usec)/1000000);
        printf("--run time : %g s\n", run_sec+run_usec);
    }

    if (ARGS[ARGS_OUT]) {
        for (int i=0;i<N;i++) {
            printf("%d ", nms[i]);
        }
        printf("\n");
    }

    free(nms);
    nms = NULL;

    return 0;
}

