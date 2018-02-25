#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv) {
    
    pid_t pid = 0;
    int status = 0;

    pid = fork();
    if (pid<0) {
        perror("fork");
        return -1;
    }
    else if (pid>0) {
        printf("I am parent, waiting child %d exit...\n",pid);
        printf("%d exited.\n",wait(&status));
    }
    else {
        printf("I am child, I will sleep 2 seconds.\n");
        sleep(2);
        exit(0);
    }
    return 0;
}

