#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv) {

    uid_t uid,euid;

    uid = getuid();
    euid = geteuid();
    
    printf("UID:%d\nEUID:%d\n",uid,euid);
    seteuid(0);
    return 0;
}

