#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void arrange(int *arr, int k, int n, char *str[])
{
    int tmp = 0;
    if (k==n-1) {
        for (int i=0;i<n;i++) {
            printf("%s ",str[arr[i]]);
        } 
        printf("\n");
    }
    else {
        for (int j=k;j<n;j++) {
            swap(arr+k, arr+j);
            arrange(arr, k+1, n, str);
            swap(arr+k, arr+j);
        }
    }
}


int
main(int argc, char *argv[])
{
    if (argc==1) {
        printf("enter something str,usage arrange ab bc cd\n");
        return 0;
    }

    int *arr = (int*)malloc(sizeof(int)*(argc-1));
    if (arr==NULL) {
        perror("malloc");
        return -1;
    }
    for (int i=0;i<argc-1;i++) {
        arr[i] = i;
    }

    arrange(arr, 0, argc-1, argv+1);
    free(arr);

    return 0;
}
