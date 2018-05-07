#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define     ARGS_ALL        0
#define     ARGS_LINK       1
#define     ARGS_SIZE       2
#define     ARGS_MODE       3
#define     ARGS_FTIME      4
#define     ARGS_UID        5
#define     ARGS_GID        6
#define     ARGS_REGEX      7
#define     ARGS_STATIS     8

#define     ARGS_END        9

static _args[ARGS_END] = {0};

struct file_list{
    char path[];
    struct stat file_stat;
    struct file_list *next;
};


int
main(int argc, char *argv[])
{
    lstat
    return 0;
}


