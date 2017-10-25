 
#include <stdio.h>
#include <string.h>
 
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
 
int main(int argc, char **argv){
    char arr1[] = {'H','E','Y','!', 0};
    map(arr1, 5, censor);
    printf("%s\n", arr1); /* HEY. */
}