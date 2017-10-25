#include <stdio.h>

int main(int argc, char** argv){
    int iarray[] = {1,2,3};
    char carray[] = {'a','b','c'};
    int* iarrayPtr=iarray;
    char* carrayPtr=carray;
    int i;
    for(i=0; i<3; i++){
        printf("%i", *(iarrayPtr+i));
    }
    printf("\n");
    for(i=0; i<3; i++){
        printf("%c", *(carrayPtr+i));
    }
    printf("\n");
    void* p;
    printf("%p\n", p);
    return 0;
}