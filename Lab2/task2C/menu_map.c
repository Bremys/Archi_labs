 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char censor(char c) {
  if(c == '!')
    return '.';
  else
    return c;
}
 
void map(char *array, int array_length, char (*f) (char)){
    int i;
    for(i=0; i<array_length; i++){
        array[i] = (*f)(array[i]);
    }
}
 
char encrypt(char c){
    if(c>=0x20 && c<=0x7E){
        c=c+3;
    }
    return c;
}
char decrypt(char c){ 
    if(c>=0x20 && c<=0x7E){
        c=c-3;
    }
    return c;
}
char xprt(char c){ 
    printf("0x%01X\n", c);
    return c;
}
char cprt(char c){
    if(c>=0x20 && c<=0x7E){
        printf("%c\n", c);
    }
    else {
        printf(".");
    }
    return c;
}
char my_get(char c){
    return fgetc(stdin);
}

char quit(char c){
    exit(c);
}
 
int main(int argc, char **argv){
    int base_len = 5;
    char arr1[base_len];
    map(arr1, base_len, my_get);
    map(arr1, base_len, encrypt);
    map(arr1, base_len, xprt);
    map(arr1, base_len, decrypt);
    map(arr1, base_len, cprt);

}