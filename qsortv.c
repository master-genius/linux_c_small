#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>


//void qsort(void *d, unsigned int size, int (*call)(void * a, void *b));

void qsort(void *d, int start, int end, int size, int(*cmp)(void *a, void *b)) {
    if (start >= end) {
        return ;
    }

    int med = (end+start)/2;
    void * center = d+med;
    int i = start;
    int j = end;
    char tmp ;
    char * buf = NULL;
    char * buf2 = NULL;

    while(1) {
        if (cmp(d+i,d+j)==0 && cmp(d[i],center)==0)break;
        while(cmp(d[i],center)<0 && i<j )i++;
        while(cmp(d[j],center)>0 && j>i)j--;
        if (i<j) {
            buf = (char*)(d+i);
            buf2 = (char*)(d+j);
            for (int k=0;k<size;k++) {
                tmp = *(buf+k);
                *(buf+k) = *(buf2+k);
                *(buf2+k) = tmp;
            }
        }
        else
            break;
    }
    if (i<med && i!=j) {
        buf = (char*)(d+i);
        buf2 = (char*)(d+j);
        for (int k=0;k<size;k++) {
            tmp = *(buf+k);
            *(buf+k) = *(buf2+k);
            *(buf2+k) = tmp;
        }
    }
    qsorti(d, start, i-1, cmp);
    qsorti(d, i+1,end, cmp);
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
        srand(nms[i]);
    }
    struct timeval  start_time,end_time;
    gettimeofday(&start_time,0);

    if (ARGS[ARGS_SORT] == SORT_QUICK) {
        qsorti(nms, 0, N-1);
    } else {
        insert_sort(nms, N);
    }
    gettimeofday(&end_time,0);
    double run_sec = (double)(end_time.tv_sec - start_time.tv_sec);
    double run_usec = ((double)(end_time.tv_usec - start_time.tv_usec)/1000000);

    if (ARGS[ARGS_OUT]) {
        for (int i=0;i<N;i++) {
            printf("%d ", nms[i]);
        }
        printf("\n");
    }
    if (ARGS[ARGS_TIME]) {
        printf("--run time : %g s\n", run_sec+run_usec);
    }

    free(nms);
    nms = NULL;

    return 0;
}

