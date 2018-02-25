#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char* itoa(int nm, char *nstr);
char* ftoa(float f);
char* dtoa(double d);


int main(int argc, char* argv[]) {

    char nstr[16] = {'\0',};

    int test_nm[8] = {-123,-1,-0,0,1,10,123,123456789};

    for(int i=0;i<8;i++) {
        printf("%s\n",itoa(test_nm[i],nstr));
    }

    return 0;
}


char* 
itoa(int nm, char *nstr) {
    
    int bits = 0;
    if (nm < 0) {
        nm = -nm;
        bits = (int)log10((double)nm) + 3;
        nstr[0] = '-';
    }
    else if (nm > 0) {
        bits = (int)log10((double)nm) + 2;
    }
    else {
        nstr[1] = '\0';
        nstr[0] = '0';
        return nstr;
    }

    nstr[bits-1] = '\0';

    int i=bits-2;
    for (; i>=0; i--) {
        nstr[i] = (nm % 10) + 48;
        nm = nm/10;
        if (nm==0) {
            break;
        }
    }

    if (i>0) {
        int k=0;
        for (;i<bits-1;k++,i++) {
            nstr[k] = nstr[i];
        }
        nstr[k] = '\0';
    }
    return nstr;
}

char* 
ftoa(float f) {

}

char* 
dtoa(double d) {

}


