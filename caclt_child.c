#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

int main(int argc, char* argv[])
{
    struct timeval start_child_time;
    struct timeval end_child_time;

    gettimeofday(&start_child_time,0);

    int child_status = 0;
    int child = fork();
    
    if (child>0) {
        if (wait(&child_status)!=child) {
            perror("wait");
            return -1;
        }

        gettimeofday(&end_child_time,0);
        
        double run_sec = (double)(end_child_time.tv_sec - start_child_time.tv_sec);
        double run_usec = ((double)(end_child_time.tv_usec - start_child_time.tv_usec))/1000000;
        
        printf("I am parent, child run time: %g\n",run_sec+run_usec);

    } else if (child==0) {
        printf("I am child.\n");
        printf("preparing to run ls command.\n");
        usleep(20000);
        char* a[4] = {0};
        a[0] = "ls";
        a[1] = "./";
        if (argc>1) {
            a[1] = argv[1];
        }
        a[2] = "-RF";

        if (argc>2) {
            a[2] = argv[2];
        }

        execv("/bin/ls",a);

    } else {
        perror("fork:");
        return -1;
    }

    return 0;
}

