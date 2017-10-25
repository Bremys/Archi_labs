#include <stdio.h>
#include <stdlib.h>

int digit_cnt (char* str){
    int counter=0, i;
    for (i=0; str[i]!=0; i++){
        if (str[i]>='0' && str[i]<='9'){
            counter++;
        }
    }
    return counter;
}

int main (int argc, char** argv){
    if(argc==1){
        fprintf(stderr, "%s\n", "Error: At least one argument must be received");
        return -1;
    }
    int counter = digit_cnt(argv[1]);
    printf("%s %d\n", "The number of digits in the string is:", counter);
    return 0;
    
}