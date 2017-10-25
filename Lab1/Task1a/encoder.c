#include <stdio.h>
#include <string.h>

int main(int argc, char** argv){
    int i, j;
    for(i=1; i<argc; ++i){
        for(j=0; j<strlen(argv[i]); ++j){
            if(argv[i][j]>='A' && argv[i][j]<='Z')
                argv[i][j] +=('a'-'A');
        }
    }
    for (i=1; i<argc; ++i){
        printf("%s ", argv[i]);
    }
    printf("\n");
    return 0;
}