#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define QN      8
#define QNS     QN*QN
#define QEND    ((QN&1)?(int)(QN/2+1):(QN/2))

static int _total_count = 0;

struct result_list {
    char pos[QN];
    struct result_list *next;
};

void
out_qnm(char *qnm);

void
set_qnm(char *qnm, int start_x, int start_y, int val);

int
find_able(char *qnm, int start_x, int start_y, int *ax);

int
run_result(char *qnm, char *pos, int pos_k, struct result_list *rlist);

int main(int argc, char* argv[])
{
    char qnm[QNS+1]={0,};
    char position[QN] = {0};
    int k=0,x=0;
    
    struct result_list pos_list, pos_head;

    pos_list.next = NULL;
    pos_head = pos_list;
    
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

    for(int i=0; i<QN; i++) {
        pos_head.pos[i] = position[i];
    }
    for (int i=0;i<QNS;i++) {
        qnm[i] = 0;
    }
    for (k=0;k<QN;k++) {
        set_qnm(qnm, position[k], k, 1);
        //qnm[k*QN+position[k]] = 33;
    }
    /*
    for (k=0;k<QN;k++) {
        for (int i=0;i<QN;i++) {
            printf("%c ",qnm[k*QN+i]+48);
        }
        printf("\n");
    }*/

    run_result(qnm, position, QN-1, &pos_list);
    struct result_list *r;
    for(r=pos_head.next;r!=NULL;r=r->next) {
        for(int p=0;p<QN;p++) {
            printf("%d ",r->pos[p]);
        }
        printf("\n");
    }
    
    for (k=0; k<QN; k++) {
        printf("%d ", pos_head.pos[k]);
        qnm[k*QN+pos_head.pos[k]] = 'Q';
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
            printf("[%s]",(qnm[k*QN+i]=='Q')?"Q":" ");
        }
        printf("\n");
    }
    printf("\n");
}

int
run_result(char *qnm, char *pos, int pos_k, struct result_list *rlist)
{
    int k = pos_k;
    int i=0;
    int ret_size = sizeof(struct result_list);
    _total_count = 0;

    while (k>=0 && pos[0]<QEND) {
        printf("k -> %d\n",k);
        set_qnm(qnm, pos[k], k, -1);
        pos[k] = find_able(qnm, pos[k]+1, k, NULL);
        if (pos[k]==-1) {
            k--;
            continue;
        }
        if (k==QN-1) {
            for (i=0;i<QN;i++) {
                printf("%d ",pos[i]);
            }
            printf("\n");
            if (_total_count>128) {
                printf("out mem\n");
                return -1;
            }
            struct result_list *result = (struct result_list*)malloc(ret_size);
            if (result==NULL) {
                perror("malloc");
                return -1;
            }
            _total_count += 1;
            result->next = NULL;
            rlist->next = result;
            rlist = result;
            for (i=0;i<QN;i++) {
                rlist->pos[i] = pos[i];
            }
            k--;
            continue;
        }
        set_qnm(qnm, pos[k], k, 1);
        k++;
    }
    
}

