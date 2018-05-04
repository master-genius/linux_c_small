#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <time.h>
#include <regex.h>
#include <errno.h>
#include <fcntl.h> 

/*
    显示文件和目录的信息，包括路径、大小、链接文件指向，统计文件数量，总大小 
*/

#define BLOCK_SIZE      512 
#define LNK_BUF     1024


#define NAM_MLEN    512

/*
    参数选项的宏定义，数字是_args的下标 
*/

#define FST_SIZE        0  // -s
#define FST_PATH        1  // -p
#define FST_LINK        2  // -l
#define FST_STATS       3  // -t
#define FST_RECU        4  // -r
#define FST_SELF        5  // -D  仅仅显示目录的信息而不是目录的内容
#define FST_REGEX       6  // -e

#define FST_ALL         15 // -a

#define MAX_ARGS        16  //最大参数个数

#define MAX_NAME_LEN    255 //最大名称字符长度

char _args[MAX_ARGS] = {0,};

struct path_list
{
    char* path;
    unsigned int plen;
    struct path_list*next;
};


struct file_ctl
{
    unsigned long long dir_count;
    unsigned long long file_count;
    unsigned long long link_count;
    unsigned long long fifo_count;
    unsigned long long blk_count;
    unsigned long long sock_count;
    unsigned long long chr_count;
    unsigned long long total_count;
    unsigned long long total_size;
    struct path_list* pl;
};

struct file_info
{
    char name[MAX_NAME_LEN];
    //char* path;
    //char* target;
    int err;
    unsigned long long size;
    unsigned long long blocks;
    struct stat st;
};

struct list
{
    char*wh;
    struct list* next;
};

char*pat;
regex_t rg[1];
regmatch_t rm[1];
char tm_type=0;


//显示目录文件，并根据参数显示对应的信息
int listdir(char *pathname, );

//递归目录，此函数调用listdir函数实现
int recur_dir_st(struct file_ctl*fc, char*path, int lflag, int sflag);

void init_fctl(struct file_ctl*);

void destroy_list(struct list*);

void destroy_plist(struct path_list*);

void help();

int out_info(struct file_info*,char*,int);

void out_count(struct file_ctl);

void parse_args(int argc, char *argv[]);

#define TIME_ACCE   1
#define TIME_MOFY   2
#define TIME_CHAG   3
#define TIME_ALL    4

int main(int argc, char* argv[])
{
    if(argc<2){
        help();
        return 0;
    }

    struct file_ctl fc;
    struct list*pl=NULL,*p=NULL;
    struct list*fl=NULL,*f=NULL;

    init_fctl(&fc);

    int i=0;
    struct  file_info fst;

    for(i=1;i<argc;i++)
    {
        if(strcmp(argv[i],"-r")==0)
            ls_flag |= LS_REC;
        else if(strcmp(av[i],"-tc")==0)
            ls_flag |= LS_TSC;
        else if(strcmp(av[i],"-c")==0)
            ls_flag |= LS_CNT;
        else if(strcmp(av[i],"-ld")==0)
            ls_flag |= LS_DIR;
        else if(strcmp(av[i],"-h")==0)
            ls_flag |= LS_HELP;
        else if(strcmp(av[i],"-rf")==0)
            ls_flag |= LS_RFIL;
        else if(strcmp(av[i],"-rd")==0)
            ls_flag |= LS_RDIR;
        else if(strcmp(av[i],"-ig")==0)
            ls_flag |= LS_IGE; 
        else if(strcmp(av[i],"-ec")==0)
            ls_flag |= LS_ESC;
        else if(strcmp(av[i],"-p")==0)
        {
            ls_flag |= LS_REG;
            ls_flag |= LS_RFIL;
            i++;
            if(i>=ac)
            {
                printf("No regex\n");
                return -1;
            }
            pat=av[i];
        }      
        else if(strcmp(av[i],"-lk")==0)
            st_flag |= FG_LLNK;
        else if(strcmp(av[i],"-s")==0)
            st_flag |= FG_SIZE;
        else if(strcmp(av[i],"-b")==0)
            st_flag |= FG_BLKS;
        else if(strcmp(av[i],"-bs")==0)
            st_flag |= FG_BLKSZ;
        else if(strcmp(av[i],"-m")==0)
            st_flag |= FG_MODE;
        else if(strcmp(av[i],"-tp")==0)
            st_flag |= FG_TYPE;
        else if(strcmp(av[i],"-i")==0)
            st_flag |= FG_INODE;
        else if(strcmp(av[i],"-u")==0)
            st_flag |= FG_UID;
        else if(strcmp(av[i],"-g")==0)
            st_flag |= FG_GID;
        else if(strcmp(av[i],"-tm")==0)
            st_flag |= FG_TIME;
        else if(strcmp(av[i],"-d")==0)
            st_flag |= FG_DEVID;
        else if(strcmp(av[i],"-rd")==0)
            st_flag |= FG_RDEVID;
        else if(strcmp(av[i],"-l")==0)
            st_flag |= FG_LINKS;
        else if(strcmp(av[i],"-n")==0)
            st_flag |= FG_NAME;
        else if(strcmp(av[i],"-pt")==0)
            st_flag |= FG_PATH;
        else if(strcmp(av[i],"-a")==0)
            st_flag |= FG_ALL;
        else
        {
            if(access(av[i],F_OK|R_OK))
            {
                perror("access");
                continue;
            }
            if(lstat(av[i],&fst.st)==-1)
                continue;
            if(S_ISDIR(fst.st.st_mode))
            {
                p=(struct list*)malloc(sizeof(struct list));
                if(!p)
                {
                    perror("malloc");
                    destroy_list(pl);
                    return errno;
                }
                p->wh=av[i];
                p->next=pl;
                pl=p;
            }
            else
            {
                f=(struct list*)malloc(sizeof(struct list));
                if(!f)
                {
                    destroy_list(fl);
                    perror("malloc");
                    return errno;
                }
                f->wh=av[i];
                f->next=fl;
                fl=f;
            }
        }

    }
    if(st_flag>0)st_flag |= FG_NAME;

    if(ls_flag&LS_HELP)
    {
        help();
        return 0;
    }

    if(pat!=NULL && (LS_REG&ls_flag))
    {
        if(regcomp(rg,pat,REG_EXTENDED|REG_ICASE)!=0)
        {
            perror("regcomp");
            destroy_list(pl);
            destroy_list(fl);
            return -1;
        }
    }

    int len=0;

    for(f=fl;f!=NULL;f=f->next)
    {
        len=strlen(f->wh);
        len=(NAM_MLEN>len)?len:(NAM_MLEN);
        strncpy(fst.name,f->wh,len);
        fst.name[len]=0;
        if(lstat(f->wh,&fst.st)==-1)
            if(ls_flag&LS_IGE)
                perror("lstat");
            else continue;
        out_info(&fst,NULL,st_flag);
    }
    destroy_list(fl);

    for(p=pl;p!=NULL;p=p->next)
    {
        if(ls_flag&LS_REC)
        {
            if(recur_dir_st(&fc,p->wh,ls_flag,st_flag)==-1)continue;
        }
        else
        {
            len=strlen(p->wh);
            len=(NAM_MLEN>len)?len:(NAM_MLEN);
            strncpy(fst.name,p->wh,len);
            fst.name[len]=0;
            if(lstat(p->wh,&fst.st)==-1)
                if(ls_flag&LS_IGE)
                    perror("lstat");
                else continue;
            out_info(&fst,NULL,st_flag);
        }
    }
    destroy_list(pl);

    if(ls_flag & LS_TSC)
    {
        //if(st_flag>0 || (ls_flag&LS_ESC))
        printf("--INFO  TOTAL:\n");
        out_count(fc);
    }

    return 0;
}

void parse_args(int argc, char* argv[]) {
    for (int i=1; i<argc; i++) {
        if ()
    }
}

void init_fctl(struct file_ctl*fc)
{
    if(!fc)return ;
    bzero((void*)fc,sizeof(struct file_ctl));
    ls_flag=st_flag=0;
    pat=NULL;
    ls_flag += LS_FIL;
}

int recur_dir_st(struct file_ctl*fc, char*path, int lflag, int sflag)
{
    if(!fc || !path)return -1;

    int plen, nlen,alen;
    char* add="";

    plen = strlen(path);

    alen=0;
    if(path[plen-1]!='/'){add="/";alen=1;}

    fc->pl=(struct path_list*)malloc(sizeof(struct path_list));

    if(!fc->pl)
    {
        perror("malloc");
        return -1;
    }

    fc->pl->path = (char*)malloc(sizeof(char)*(plen+1+alen));

    if(!fc->pl->path)
    {
        free(fc->pl);
        fc->pl=NULL;
        perror("malloc");
        return -1;
    }

    fc->pl->next=NULL;
    strncpy(fc->pl->path,path,plen);
    strncpy(fc->pl->path+plen,add,alen);
    fc->pl->path[plen+alen]=0;
    fc->pl->plen=plen+alen;

    struct file_info fin;
    bzero((void*)&fin,sizeof(fin));

    struct file_ctl ef, pf;
    bzero((void*)&ef,sizeof(ef));
    bzero((void*)&pf,sizeof(pf));

    DIR* r;
    struct dirent* d;

    plen=nlen=0;
    struct path_list* p=NULL,*q=NULL,*b=NULL;
    char*buf=NULL; int buf_len=0,reg_ok=0,re=0;
    for(p=fc->pl;p!=NULL;p=p->next)
    {
        //if(!(lflag&LS_NPH)) printf("%s:\n",p->path);

        r=opendir(p->path);

        if(!r)
        {
            if(!(lflag&LS_IGE))
                perror("opendir");
            continue;
        }
        d=readdir(r);
        while(d)
        {
            if((strcmp(d->d_name,".")==0) || (strcmp(d->d_name,"..")==0))
                goto next;

            nlen=strlen(d->d_name);

            if(pat!=NULL && (lflag&LS_REG))
            {
                if((lflag&LS_RDIR) && d->d_type==DT_DIR)
                    reg_ok=1;
                else if((lflag&LS_RFIL) && d->d_type!=DT_DIR)
                    reg_ok=1;
                else reg_ok=0;
                if(reg_ok==1)
                {
                    rm->rm_so=-1;
                    re=regexec(rg,d->d_name,1,rm,0);
                    if(re==REG_NOMATCH)goto next;
                    else if(re==0)goto start;
                    else if(!(lflag&LS_IGE))perror("regexec");
                }
            }start:;

            if(d->d_type == DT_DIR)
            {
                b=(struct path_list*)malloc(sizeof(struct path_list));
                if(!b)
                {
                    perror("malloc");
                    closedir(r);r=NULL;
                    break;
                }
                b->next=NULL;
                add="/";alen=1;
                b->path = (char*)malloc(sizeof(char)*(p->plen+alen+nlen+1));
                if(!b->path)
                {
                    free(b);
                    b=NULL;
                    perror("malloc");
                    closedir(r);r=NULL;
                    break;
                }
                strncpy(b->path,p->path,p->plen);
                strncpy(b->path+p->plen,d->d_name,nlen);
                strncpy(b->path+p->plen+nlen,add,alen);
                b->plen = p->plen + alen + nlen;
                b->path[b->plen]=0;

                q=p->next;
                p->next=b;
                b->next=q;
                q=b=NULL;
            }

            if(d->d_type == DT_DIR)
                {fc->dir_count++;ef.dir_count++;pf.dir_count++;}
            else if(d->d_type == DT_REG)
                {fc->file_count++;ef.file_count++;pf.file_count++;}
            else if(d->d_type == DT_LNK)
                {fc->link_count++;ef.link_count++;pf.link_count++;}
            else if(d->d_type == DT_BLK)
                {fc->blk_count++;ef.blk_count++;pf.blk_count++;}
            else if(d->d_type == DT_CHR)
                {fc->chr_count++;ef.chr_count++;pf.chr_count++;}
            else if(d->d_type == DT_FIFO)
                {fc->fifo_count++;ef.fifo_count++;pf.fifo_count++;}
            else if(d->d_type == DT_SOCK)
                {fc->sock_count++;ef.sock_count++;pf.sock_count++;}
            fc->total_count++;ef.total_count++;pf.total_count++;

            if(d->d_type==DT_DIR && !(lflag&LS_DIR))
                goto next;
            else if(!(lflag&LS_FIL))
                goto next;
            if(!buf || buf_len<=(p->plen+nlen))
            {
                if(buf)free(buf);
                buf = (char*)malloc(sizeof(char)*(p->plen+nlen+1));
                if(!buf)
                {
                    //closedir(r);
                    perror("malloc");
                    closedir(r);
                    r=NULL;
                    break;
                }
                buf_len = p->plen+nlen+1;
            }
            strncpy(buf,p->path,p->plen);
            strncpy(buf+p->plen,d->d_name,nlen);
            buf[p->plen+nlen]=0;
            strncpy(fin.name,d->d_name,(NAM_MLEN>nlen)?nlen:NAM_MLEN);
            re=lstat(buf,&fin.st);
            if(re==-1 && !(lflag&LS_IGE))
            {
                perror("lstat");
                goto next;
            }
            fc->total_size += fin.st.st_size;
            ef.total_size += fin.st.st_size;
            pf.total_size += fin.st.st_size;
            out_info(&fin,p->path,sflag);

            next:
            bzero((void*)&fin,sizeof(fin));
            plen=nlen=0;
            d=readdir(r);
        }closedir(r);r=NULL;
        if(lflag&LS_ESC){
            printf("--INFO  %s:\n",p->path);
            out_count(ef);printf("_______\n");
            }
        bzero((void*)&ef,sizeof(ef));

    }

    if(lflag&LS_CNT)
    {
        printf("--INFO  %s:\n",path);
        out_count(pf);
    }

    if(buf)free(buf);
    buf=NULL;
    destroy_plist(fc->pl);
    return 0;
}

int out_info(struct file_info*fi,char*path,int sflag)
{
    if(!fi)return -1;

    int nl_flag = 0;
    if(sflag & FG_ALL)
        nl_flag = 1;

    if((FG_PATH & sflag) && path) 
        printf("%s",path);

    if(sflag & FG_NAME)
        printf("%s",fi->name); 

    if(S_ISLNK(fi->st.st_mode) && (sflag&FG_LLNK))
    {
        printf(" --> ");
        char link[LNK_BUF]={0,};
        char* lk=link;
        int p_len=0, n_len=0;
        if(path)
            p_len = strlen(path);
        n_len = strlen(fi->name);
        char* tp=NULL;
        tp = (char*)malloc(sizeof(char)*(p_len + n_len + 1));
        if(!tp)
        {
            perror("malloc");
            printf("error: can not get link target\n");
            goto next;
        }
        if(path)
        {
            strncpy(tp,path,p_len);
        }
        strncpy(tp+p_len,fi->name,n_len);
        tp[p_len+n_len]=0;
        
        int l_size=0, buf_size=LNK_BUF;
        lk=link;
        l_size = readlink(tp,lk,buf_size);
        if(l_size < 0)
        {
            perror("readlink");
            free(tp);
            tp=NULL;
            goto next;
        }
        if(l_size >= buf_size)
            lk=NULL;
        else
            lk[l_size]=0;
        int i; 
        while(l_size >= buf_size)
        {
            buf_size += 256;
            free(lk);
            lk = (char*)malloc(sizeof(char)*(buf_size+1));
            if(!lk)
            {
                perror("malloc");
                free(tp);
                goto next;
            }
            for(i=0;i<=buf_size;i++)
                lk[i] = 0;
            l_size = readlink(tp,lk,buf_size-1);
            if(l_size<0)
            {
                perror("readlink");
                free(lk);
                free(tp);
                tp=lk=NULL;
                goto next;
            }
            lk[buf_size]=0;
        }
        printf("%s",lk);
        free(tp);
        if(!(lk==link))
            free(lk);
        lk=tp=NULL;
    }

next: 

    if(sflag&(~(FG_PATH|FG_NAME|FG_LLNK)))
        printf(" ");

    if(FG_MODE & sflag)
        printf("%#o ",fi->st.st_mode&~(S_IFMT));
    if(FG_SIZE & sflag)
    {
        if(fi->st.st_size < 1024)
            printf("%lldb ",(long long)fi->st.st_size);
        else if(fi->st.st_size >= 1024 && fi->st.st_size < 1048576)
            printf("%.2fkb ",((double)fi->st.st_size)/1024);
        else if(fi->st.st_size >= 1048576)
            printf("%.2fmb ",(fi->st.st_size*1.0)/1048576);
    }
    if(FG_BLKS & sflag)
        printf("%lld ",(long long)fi->st.st_blocks);
    if(FG_BLKSZ & sflag)
        printf("%d ",(int)fi->st.st_blksize);
    if(FG_LINKS & sflag)
        printf("%ld ",fi->st.st_nlink);

    if(FG_TYPE & sflag)
    {
        if(S_ISREG(fi->st.st_mode))
            printf("reg ");
        else if(S_ISDIR(fi->st.st_mode))
            printf("dir ");
        else if(S_ISLNK(fi->st.st_mode))
            printf("link ");
        else if(S_ISFIFO(fi->st.st_mode))
            printf("fifo ");
        else if(S_ISSOCK(fi->st.st_mode))
            printf("sock ");
        else if(S_ISCHR(fi->st.st_mode))
            printf("chr ");
        else if(S_ISBLK(fi->st.st_mode))
            printf("blk ");
    }
    if(FG_UID & sflag)
        printf("%d ",fi->st.st_uid);
    if(FG_GID & sflag)
        printf("%d ",fi->st.st_gid);
    if(FG_DEVID & sflag)
        printf("%d-%d ",(int)major(fi->st.st_dev),(int)minor(fi->st.st_dev));
    if(FG_INODE & sflag)
        printf("%lld ",(long long)fi->st.st_ino);
    if((FG_RDEVID & sflag) &&(fi->st.st_rdev > 0))
        printf("%d",(int)fi->st.st_rdev);

    if(nl_flag)
        printf("\n");

    char*b=NULL;
    if(FG_TIME & sflag)
    {
        printf("  %s",ctime(&(fi->st.st_atime)));
        printf("  %s",ctime(&(fi->st.st_mtime)));
        printf("  %s",ctime(&(fi->st.st_ctime)));
    }

    return 0;
}

void destroy_plist(struct path_list*pl)
{
    struct path_list*p=pl;
    while(pl)
    {
        p=pl;
        pl=pl->next;
        if(p->path){free(p->path);p->path=NULL;}
        if(p){free(p);p=NULL;}
    }
    pl=NULL;
}

void destroy_list(struct list*l)
{
    struct list*p=l;
    while(p)
    {
        l=l->next;
        free(p);
        p=l;
    }
}

void out_count(struct file_ctl fc)
{
    if(fc.dir_count>0)
        printf("dirs:%lld\n",fc.dir_count);
    if(fc.file_count>0)
        printf("files:%lld\n",fc.file_count);
    if(fc.link_count)
        printf("links:%lld\n",fc.link_count);
    if(fc.sock_count)
        printf("socks:%lld\n",fc.sock_count);
    if(fc.fifo_count>0)
        printf("fifos:%lld\n",fc.fifo_count);
    if(fc.chr_count>0)
        printf("chrs:%lld\n",fc.chr_count);
    if(fc.blk_count>0)
        printf("blks:%lld\n",fc.blk_count);
    printf("total:%lld\nsize:",fc.total_count);
    if(fc.total_size < 1024)
        printf("%lldb\n",fc.total_size);
    else if(fc.total_size >= 1024 && fc.total_size < 1048576)
        printf("%.2fkb\n",((double)fc.total_size)/1024);
    else if(fc.total_size >= 1048576)
        printf("%.2fmb\n",((double)fc.total_size)/1048576);

}

void help()
{

    printf("This program list file stat.\n");
    printf("    -h:   help\n");
    printf("    -i:   list inode\n");
    printf("    -m:   list mode\n");
    printf("    -tp:  list the type of file\n");
    printf("    -d:   list device ID\n");
    printf("    -b:   list the number of blocks\n");
    printf("    -bs:  list block-size\n");
    printf("    -rd:  list rdev(if it is a special file)\n");
    printf("    -s:   list the size of file\n");
    printf("    -tm:  list the last time that operating the file\n");
    printf("    -l:   list the number of links\n");
    printf("    -u: list user id\n");
    printf("    -g: list group id\n");
    printf("    -a:   list all\n");
    printf("    -ld:  list dir stat\n");
    printf("    -lp:  list path\n");
    printf("    -n:   list file name\n");
    printf("    -c:   list the counts of each type\n");
    printf("    -r:   recursion dir\n");
    printf("    -p:   pattern in regular expression\n");
    printf("    -rd:  regex for dir\n");
    printf("    -rf:  regex for file\n");
    printf("    -pt:  list path\n");
    printf("    -tc:  total info\n");
    printf("    -ec:  list the counts of each type of every dir\n");
    printf("    -lk:  list the link target if the file is a link file\n");
    printf("    -ig:  ignore error\n");
    printf("the order: path name link(if it is)  mode size blocks block-size count of link\n");
    printf("file-type uid gid device-number inode last-access-time last-moltifly-time last-\n");
    printf("change-time\n");
}
