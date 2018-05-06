#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define     ARGS_ALL        0
#define     ARGS_COUNT      1
#define     ARGS_HELP       2
#define     ARGS_POSI       3

#define     ARGS_END        4

#define     MAX_MATRIX      64

static int _total_count = 0;
static int _max_result  = 0;

struct result_list {
    char pos[MAX_MATRIX];
    struct result_list *next;
};

void
set_qnm(char *qnm, int n, int start_x, int start_y, int val);

int
find_able(char *qnm, int n, int start_x, int start_y, int *ax);

int
run_result(char *qnm, int n, char *pos, int pos_k, struct result_list *rlist);

void
out_result(char *qnm, int n, char *pos, int flag);

void
free_retlist(struct result_list *rlist);

void
help(){
    char *help_info[] = {
        "解决经典的八皇后问题，默认输出8x8棋盘的一个解",
        "支持参数：--all, --count, --max=[MAX_LIMIT], --position, [NUMBER]",
        "  --all：此参数会在一个初始解的基础之上找出所有解。",
        "  --count：只输出结果计数，并不输出结果，要和--all配置使用。",
        "  --max=[MAX_LIMIT]：最大计算结果，此参数用于限制结果数量，对于",
        "    特别大的解集，可以只显示部分结果而不是一直计算下去。",
        "  --position：输出位置信息。",
        "  [NUMBER]：直接跟一个数字表示计算NxN的棋盘，比如，qnsa2 16",
        "示例：",
        "  qnsa2 --all --count 15",
        "  qnsa2 --all 13",
        "  qnsa2 --all --max=20 18",
        "此版本为优化版本，最大支持64x64的棋盘",
        "\0"
    };
    int i=0;
    while (1) {
        if (strcmp(help_info[i],"\0")==0){
            break;
        }
        printf("%s\n",help_info[i]);
        i++;
    }
}

int main(int argc, char* argv[])
{
    int status = 0;
    char args[ARGS_END] = {0};
    int n = 8;

    for (int i=1; i<argc; i++) {
        if (strcmp(argv[i],"--all")==0) {
            args[ARGS_ALL] = 1;
        }
        else if(strcmp(argv[i],"--count")==0) {
            args[ARGS_COUNT] = 1;
        }
        else if(strncmp(argv[i],"--max=",6)==0) {
            if (strlen(argv[i])>6) {
                _max_result = atoi(argv[i]+6);
                _max_result = (_max_result>=0)?_max_result:0;
            }
        }
        else if(strcmp(argv[i],"--help")==0) {
            args[ARGS_HELP] = 1;
        }
        else if(strcmp(argv[i],"--position")==0) {
            args[ARGS_POSI] = 1;
        }
        else {
            n = atoi(argv[i]);
            n = ((n>5 && n<=64)?n:8);
        }
    }

    if (args[ARGS_HELP]) {
        help();
        return 0;
    }

    int qns = n*n;
    char *qnm = (char*)malloc(sizeof(char)*qns);
    char *position = (char*)malloc(sizeof(char)*n);

    int k=0,x=0;
    
    struct result_list pos_head;
    pos_head.next = NULL;
    
    int qend = ((n&1)?(n/2+1):(n/2));

    for (int i=0;i<qns;i++) {
        qnm[i] = 0;
    }

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
    out_result(qnm, n, pos_head.pos, args[ARGS_POSI]);
    if (args[ARGS_ALL]==0) {
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
        out_result(qnm, n, r->pos, args[ARGS_POSI]);
    }

    free_retlist(pos_head.next);
end_result:;
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
    int old_posi;
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

            _total_count += 1;
            result->next = NULL;
            rlist->next = result;
            rlist = result;
            for (i=0;i<n;i++) {
                rlist->pos[i] = pos[i];
            }

            if (_max_result>0 && _total_count>=_max_result) {
                return 0;
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
out_result(char *qnm, int n, char *pos, int flag)
{
    int i=0,k=0;
    int qns = n*n;

    for (i=0;i<qns;i++) {
        qnm[i] = ' ';
    }

    for (k=0;k<n;k++) {
        qnm[k*n+pos[k]] = 'Q';
    }

    for (k=0;k<n;k++) {
        for (i=0;i<n;i++) {
            printf("|%c",qnm[k*n+i]);
        }
        if (flag==1) {
            printf("|%d\n",pos[k]);
        } else {
            printf("|\n");
        }
    }
    printf("\n");
}

