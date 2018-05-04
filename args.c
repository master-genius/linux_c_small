#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int i=0;
    printf("%d\n", argc);
    if (argc>=2) {
        if (!strcmp(argv[1],"hello")) {
            printf("Hello world\n");
        }
        else if (!strcmp(argv[1],"what")) {
            printf("I am %s\n", argv[0]);
        }
    }
    else{
        printf("nothing\n");
    }
    return 0;
}

