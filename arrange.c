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

void arrange(int *arr, int k, int n)
{
    int tmp = 0;
    if (k==n-1) {
        for (int i=0;i<n;i++) {
            printf("%d ",arr[i]);
        } 
        printf("\n");
    }
    else {
        for (int j=k;j<n;j++) {
            swap(arr+k, arr+j);
            arrange(arr, k+1, n);
            swap(arr+k, arr+j);
        }
    }
}


int
main(int argc, char *argv)
{
    int arr[10] = {1,2,3,4,5,6,7,8,9,10};
    arrange(arr, 0, 10);
    return 0;
}
