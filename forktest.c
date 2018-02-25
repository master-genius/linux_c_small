#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    
    pid_t pid = 0;
    pid = fork();

    if (pid<0) {
        perror("fork");
    }
    else if (pid>0) {
        printf("I am parent, child %d running\n",pid);
    }
    else {
        printf("I am child\n");
    }
    return 0;
}

