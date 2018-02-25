#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

int main(int argc, char* argv[])
{
    int fd = open("stdio_test",O_APPEND|O_CREAT|O_RDWR, S_IRWXU);
    
    printf("%d\n", fd);
    
    char data[50] = "io - ";
    //time_t tm = 
    char *t = itoa(time(NULL)); 
    strcat(data, t);

    write(fd, data, strlen(data));
    
    close(fd);
    
    write(STDOUT_FILENO, "1234567 - hello\n", 20);

    return 0;
}

