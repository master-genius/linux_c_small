#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/fs.h>

int main(int argc, char *argv[])
{
    pid_t pid = -1;

    pid = fork();
    if (pid<0) {
        return -1;
    }
    else if (pid>0) {
        exit(EXIT_SUCCESS);
    }
    printf("start daemon...\n");

    if (setsid()==-1) {
        perror("setsid");
        return -1;
    }

    if (chdir("/")==-1) {
        perror("chdir");
        return -1;
    }

    printf("daemon running pid: %d\n",getpid());

    close(0);
    close(1);
    close(2);
    open("/dev/null",O_RDWR);
    dup(0);
    dup(0);

    while(1) {
        sleep(1);
    }

    return 0;
}

