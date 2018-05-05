#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void
out_qnm(char *qnm, int n);

void
set_qnm(char *qnm, int n, int start_x, int start_y, int val);

int
find_able(char *qnm, int n, int start_x, int start_y, int *ax);

int main(int argc, char* argv[])
{
    int n = 8;
    if (argc > 1) {
        n = atoi(argv[1]);
        if (n<8) {
            n = 8;
        }
    }

    int qend = ((n&1)?(n/2+1):(n/2));

    char *qnm = (char*)malloc(sizeof(char)*(n*n+1));
    char *position = (char*)malloc(sizeof(char)*n);
    int k=0,x=0;
    
    while(k<n) {
        position[k] = find_able(qnm, n, x, k, NULL);
        if (position[k]==-1) {
            k -= 1;
            set_qnm(qnm, n, position[k], k, -1);
            x = position[k]+1;
            continue;
        }
        else if (k==0 && position[k]>qend) {
            printf("not solution\n");
            goto not_result;
        }
        x=0;
        set_qnm(qnm, n, position[k], k, 1);
        k++;
    }
    
    for (k=0; k<n; k++) {
        printf("%d ", position[k]);
        qnm[k*n+position[k]] = 'Q';
    }
    printf("\n");
    out_qnm(qnm, n);
not_result:;
    free(qnm);
    free(position);
    qnm = position = NULL;
    return 0;
}

void
set_qnm(char *qnm, int n, int start_x, int start_y, int val)
{
    int i=0,k=0;
    if (val > 0){
        qnm[start_y*n+start_x] = 1;
    }
    else if (val < 0) {
        qnm[start_y*n+start_x] = 0;
    }

    for (k=start_y+1; k<n; k++) {
        qnm[k*n+start_x] += val;
    }

    for (k=start_y+1, i=start_x+1; k<n && i<n; i++,k++) {
        qnm[k*n+i] += val;
    }
    
    for (k=start_y+1, i=start_x-1; i>=0 && k<n; i--,k++) {
        qnm[k*n+i] += val;
    }

}

int
find_able(char *qnm, int n, int start_x, int start_y, int *ax)
{
    for (int i=start_x; i<n; i++) {
        if (qnm[start_y*n+i] == 0) {
            if (ax!=NULL){*ax = i;}
            return i;
        }
    }
    return -1;
}

void
out_qnm(char *qnm, int n)
{
    for(int k=0; k<n; k++) {
        for (int i=0; i<n; i++) {
            printf("|%s",(qnm[k*n+i]=='Q')?"Q":" ");
        }
        printf("|\n");
    }
    printf("\n");
}

