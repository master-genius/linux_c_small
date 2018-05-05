#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int _total_count = 0;

struct result_list {
    char *pos;
    struct result_list *next;
};

void
set_qnm(char *qnm, int n, int start_x, int start_y, int val);

int
find_able(char *qnm, int n, int start_x, int start_y, int *ax);

int
run_result(char *qnm, int n, char *pos, int pos_k, struct result_list *rlist);

void
out_result(char *qnm, int n, char *pos);

void
free_retlist(struct result_list *rlist);


int main(int argc, char* argv[])
{
    int status = 0;
    char args[2] = {0,0};
    int n = 8;

    for (int i=1; i<argc; i++) {
        if (strcmp(argv[i],"--all")==0) {
            args[0] = 1;
        }
        else if(strcmp(argv[i],"--count")==0) {
            args[1] = 1;
        }
        else {
            n = atoi(argv[i]);
            n = ((n>8)?n:8);
        }
    }

    if (argc > 1) {
        n = atoi(argv[1]);
        n = ((n>8)?n:8);
    }

    char *qnm = (char*)malloc(sizeof(char)*n*n);
    char *position = (char*)malloc(sizeof(char)*n);

    int k=0,x=0;
    
    struct result_list pos_head;
    pos_head.next = NULL;
    pos_head.pos = (char*)malloc(sizeof(char)*n);
    
    int qend = ((n&1)?(n/2+1):(n/2));
    int qns = n*n;

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
            goto end_result;
        }
        x=0;
        set_qnm(qnm, n, position[k], k, 1);
        k++;
    }

    for(int i=0; i<n; i++) {
        pos_head.pos[i] = position[i];
    }

    printf("original result:\n");
    out_result(qnm, n, pos_head.pos);
    if (args[0]==0) {
        goto end_result;
    }

    for (int i=0;i<qns;i++) {
        qnm[i] = 0;
    }
    for (k=0;k<n;k++) {
        set_qnm(qnm, n, position[k], k, 1);
    }

    int ret = run_result(qnm, n, position, n-1, &pos_head);
    if (ret==-1) {
        status = -1;
        goto end_result;
    }
    printf("others:%d counts\n",_total_count);
    printf("total counts: %d\n", _total_count+1);
    printf("with mirror counts: %d\n",(_total_count+1)*2);
    if (args[1]==1) {
        goto end_result;
    }

    struct result_list *r;
    for(r=pos_head.next;r!=NULL;r=r->next) {
        out_result(qnm, n, r->pos);
    }

    free_retlist(pos_head.next);
end_result:;
    free(pos_head.pos);
    free(position);
    free(qnm);
    qnm = position = NULL;

    return status;
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

int
run_result(char *qnm, int n, char *pos, int pos_k, struct result_list *rlist)
{
    int k = pos_k;
    int i=0;
    int old_k, old_posi;
    int ret_size = sizeof(struct result_list);
    _total_count = 0;
    struct result_list *result = NULL;
    
    int qend = ((n&1)?(n/2+1):(n/2));

    while (k>=0 && pos[0]<qend) {
        old_posi = pos[k];
        pos[k] = find_able(qnm, n, pos[k]+1, k, NULL);
        if (pos[k]==-1) {
            if (old_posi>=0)
                set_qnm(qnm, n, old_posi, k, -1);
            k -= 1;
            continue;
        }
        set_qnm(qnm, n, pos[k], k, 1);
        if (old_posi>=0)
            set_qnm(qnm, n, old_posi, k, -1);
        k++;
        if (k==n) {
            k--;
            result = (struct result_list*)malloc(ret_size);
            if (result==NULL) {
                perror("malloc");
                return -1;
            }
            result->pos = (char*)malloc(sizeof(char)*n);
            if (result==NULL) {
                perror("malloc");
                free(result);
                return -1;
            }

            _total_count += 1;
            result->next = NULL;
            rlist->next = result;
            rlist = result;
            for (i=0;i<n;i++) {
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
        free(p->pos);
        free(p);
        p = q;
    }
}

void
out_result(char *qnm, int n, char *pos)
{
    int i=0,k=0;
    int qns = n*n;

    for (i=0;i<qns;i++) {
        qnm[i] = ' ';
    }

    for (k=0;k<n;k++) {
        qnm[k*n+pos[k]] = 'Q';
        printf("%d ", pos[k]);
    }
    printf("\n");

    for (k=0;k<n;k++) {
        for (i=0;i<n;i++) {
            printf("|%c",qnm[k*n+i]);
        }
        printf("|\n");
    }
    printf("\n");
}

