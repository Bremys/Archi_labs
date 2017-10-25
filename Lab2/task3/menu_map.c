 
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
    printf("0x%X\n", c);
    return c;
}
char cprt(char c){
    if(c>=0x20 && c<=0x7E){
        printf("%c\n", c);
    }
    else {
        printf(".\n");
    }
    return c;
}
char my_get(char c){
    return fgetc(stdin);
}

char quit(char c){
    exit(c);
}

typedef struct fun_desc {
  char *name;
  char (*fun)(char);
}fun_desc;

fun_desc menu[] = { { "Censor", censor}, { "Encrypt", encrypt}, { "Decrypt", decrypt}, { "Print hex", xprt}, { "Print string", cprt}, { "Get String", my_get}, { "Quit", quit}, { NULL, NULL } };
 


int menulen(){
    int i=0;
    while(menu[i].name!=NULL){
        i++;
    }
    return i;
}

void printMenu(){
    int i;
    printf("Choose a function: \n");
    for(i=0; i<menulen(); i++){
        printf("%i) %s\n", i, menu[i].name);
    }
    printf("Option: ");
}

int main(int argc, char **argv){
    char* carray = (char*)calloc(5, sizeof(char));
    printMenu();
    int option = fgetc(stdin)-'0';
    while(option>=0 && option <menulen()){
        fgetc(stdin);
        printf("Within bounds\n");
        map(carray, 5, menu[option].fun);
        printf("DONE.\n");
        printMenu();
        option = fgetc(stdin)-'0';
    }
    free(carray);
    printf("Not within bounds");
    return 0;
}