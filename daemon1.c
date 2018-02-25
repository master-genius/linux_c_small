#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv) {
    
    pid_t pid = 0;

    pid = fork();

    if(pid<0) {
        perror("fork");
        return -1;
    }
    else if (pid>0) {
        sleep(5);
    }
    else {
        sleep(10);
    }

    return 0;
}
