#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define QN      8
#define QNS     QN*QN
#define QEND    ((QN&1)?(int)(QN/2+1):(QN/2))

void
out_qnm(char *qnm);

void
set_qnm(char *qnm, int start_x, int start_y, int val);

int
find_able(char *qnm, int start_x, int start_y, int *ax);

int main(int argc, char* argv[])
{
    char qnm[QNS+1]={0,};
    char position[QN] = {0};
    int k=0,x=0;
    
    while(k<QN) {
        position[k] = find_able(qnm, x, k, NULL);
        if (position[k]==-1) {
            k -= 1;
            set_qnm(qnm, position[k], k, -1);
            x = position[k]+1;
            continue;
        }
        else if (k==0 && position[k]>QEND) {
            printf("not solution\n");
            goto not_result;
        }
        x=0;
        set_qnm(qnm, position[k], k, 1);
        k++;
    }
    
    for (k=0; k<QN; k++) {
        printf("%d ", position[k]);
        qnm[k*QN+position[k]] = 'Q';
    }
    printf("\n");
    out_qnm(qnm);
not_result:;
    return 0;
}

void
set_qnm(char *qnm, int start_x, int start_y, int val)
{
    int i=0,k=0;
    if (val > 0){
        qnm[start_y*QN+start_x] = 1;
    }
    else if (val < 0) {
        qnm[start_y*QN+start_x] = 0;
    }

    for (k=start_y+1; k<QN; k++) {
        qnm[k*QN+start_x] += val;
    }

    for (k=start_y+1, i=start_x+1; k<QN && i<QN; i++,k++) {
        qnm[k*QN+i] += val;
    }
    
    for (k=start_y+1, i=start_x-1; i>=0 && k<QN; i--,k++) {
        qnm[k*QN+i] += val;
    }

}

int
find_able(char *qnm, int start_x, int start_y, int *ax)
{
    for (int i=start_x; i<QN; i++) {
        if (qnm[start_y*QN+i] == 0) {
            if (ax!=NULL){*ax = i;}
            return i;
        }
    }
    return -1;
}

void
out_qnm(char *qnm)
{
    for(int k=0; k<QN; k++) {
        for (int i=0; i<QN; i++) {
            printf("|%s",(qnm[k*QN+i]=='Q')?"Q":" ");
        }
        printf("|\n");
    }
    printf("\n");
}

