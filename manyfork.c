#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int _fork_num = 5;

int
main(int argc, char *argv[])
{
    int pid = 0;
    static int _fork_count = 0;
start_fork:;
    
     pid = fork();
     if (pid==-1) {
        perror("fork");
        return -1;
     }

     if (pid>0) {
        if (_fork_count<_fork_num) {
            _fork_count++;
            goto start_fork;
        }
        while(1) {
            sleep(1);
        }
     }
     else {
        while(1) {
            printf("Child %d running, hello.\n",getpid());
            sleep(1);
        }
     }

    return 0;
}
