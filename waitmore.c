#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv) {
    
    pid_t pid = 0;
    int status = 0;

    for (int i=0; i<5; i++) {
        pid = fork();
        if (pid==-1) {
            perror("fork");
            return -1;
        }
        else if (pid>0) {
            printf("parent fork child:%d\n", pid);
        }
        else {
            printf("child:%d, waiting exit..\n",getpid());
            sleep(i+2);
            exit(0);
        }
    }
    for(int i=0;i<5;i++) {
        printf("%d exited\n",wait(&status));
    }

    return 0;
}

