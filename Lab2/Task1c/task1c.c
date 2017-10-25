#include <stdio.h>

int main(int argc, char** argv){
    int iarray[3];
    char carray[3];
    printf("%p\n", iarray);
    printf("%p\n", iarray+1);
    printf("%p\n", carray);
    printf("%p\n", carray+1);
    return 0;
}