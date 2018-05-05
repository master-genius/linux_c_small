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

void
out_result(char *qnm, char *pos);

void
free_retlist(struct result_list *rlist);


int main(int argc, char* argv[])
{
    char qnm[QNS+1]={0,};
    char position[QN] = {0};
    int k=0,x=0;
    
    struct result_list pos_list, pos_head;
    pos_head.next = NULL;
    
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
    }

    printf("original result:\n");
    for (k=0; k<QN; k++) {
        printf("%d ", pos_head.pos[k]);
        qnm[k*QN+pos_head.pos[k]] = 'Q';
    }
    printf("\n");
    out_qnm(qnm);

    int ret = run_result(qnm, position, QN-1, &pos_head);
    if (ret==-1) {
        free_retlist(pos_head.next);
        return -1;
    }
    printf("others:%d count\n",_total_count);
    
    struct result_list *r;
    for(r=pos_head.next;r!=NULL;r=r->next) {
        out_result(qnm, r->pos);
    }

    free_retlist(pos_head.next); 

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

int
run_result(char *qnm, char *pos, int pos_k, struct result_list *rlist)
{
    int k = pos_k;
    int i=0;
    int old_k, old_posi;
    int ret_size = sizeof(struct result_list);
    _total_count = 0;
    struct result_list *result = NULL;
    int qend = QEND;
    while (k>=0 && pos[0]<qend) {
        old_posi = pos[k];
        pos[k] = find_able(qnm, pos[k]+1, k, NULL);
        if (pos[k]==-1) {
            if (old_posi>=0)
                set_qnm(qnm, old_posi, k, -1);
            k -= 1;
            continue;
        }
        set_qnm(qnm, pos[k], k, 1);
        if (old_posi>=0)
            set_qnm(qnm, old_posi, k, -1);
        k++;
        if (k==QN) {
            k--;
            result = (struct result_list*)malloc(ret_size);
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
        }
    }
    return 0; 
}

void
free_retlist(struct result_list *rlist)
{
    struct result_list *p=rlist;
    struct result_list *q=rlist;
    while (p!=NULL) {
        q = q->next;
        free(p);
        p = q;
    }
}

void
out_result(char *qnm, char *pos)
{

    int i=0,k=0;
    for (i=0;i<QNS;i++) {
        qnm[i] = ' ';
    }

    for (k=0;k<QN;k++) {
        qnm[k*QN+pos[k]] = 'Q';
    }

    for (k=0;k<QN;k++) {
        for (i=0;i<QN;i++) {
            printf("|%c",qnm[k*QN+i]);
        }
        printf("|\n");
    }
    printf("\n");
}

