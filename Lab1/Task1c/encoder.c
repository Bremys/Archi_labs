#include <stdio.h>
#include <string.h>

int main(int argc, char** argv){
    int i, key=0;
    FILE* inFile=stdin;
    char currentC;
    for(i=1; i<argc; ++i){
        if((argv[i][0]=='+' || argv[i][0] == '-') && (strcmp(argv[i], "-i")!=0)){
            key=argv[i][1]-'0';
            if(argv[i][0]=='-')
                key = key*(-1);
        }
    }
    for(i=1; i<argc; ++i){
        if(strcmp(argv[i], "-i")==0){
            inFile = fopen(argv[i+1], "r");
            if(inFile == NULL){
                printf("file not found");
                return(-1);
            }
        }
    }
    currentC=fgetc(inFile);
    while(!feof(inFile)){
        if(currentC>='A' && currentC<='Z'){
            currentC +=('a'-'A');
        }
        if(currentC>='a' && currentC<='z'){
            currentC+=key;
        }
        printf("%c", currentC);
        currentC=fgetc(inFile);
    }
    if(inFile!=stdin){
        fclose(stdin);
    }
    return 0;
}