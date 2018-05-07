#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    int n = 30;
    int m = 15;

    for (int i=1;i<argc;i++) {
        if (strncmp(argv[i],"-n=",3)==0) {
            if (strlen(argv[i])>3) {
                n = atoi(argv[i]+3);
                n = (n>1)?n:30;
            }
        }
        else if (strncmp(argv[i],"-m=",3)==0) {
            if (strlen(argv[i])>3) {
                m = atoi(argv[i]+3);
                m = (m>1)?m:15;
            }
        }
    }

    int *loop = (int*)malloc(sizeof(int)*(n+1));
    if(loop==NULL)
    {
        perror("malloc");
        return -1;
    }
    
    int count = 0;

    for (int i=0;i<=n;i++) {
        loop[i] = 0;
    }

    int death=0,i=0;
    int live = n;
    while(live>0){
        
        death++;
        if (death>n){
            death = 1;
        }

        if(!loop[death]) {
            count++;
        }

        if (count==m) {
            loop[death] = 1;
            printf("%d ",death);
            count = 0;
            live--;
        }
       
    }
    printf("\n");

    free(loop);
    loop = NULL;
    return 0;
}
