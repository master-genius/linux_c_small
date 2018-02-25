#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    
    int pid = getpid();
    int ppid = getppid();

    printf("pid: %d -- ppid: %d\n",pid,ppid);

    return 0;
}

