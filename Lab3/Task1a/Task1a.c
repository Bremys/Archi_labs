
#include <stdio.h>
#include <stdlib.h>

void print_hex(char* buffer, int length){
    int i;
    for(i=0; i<length; ++i){
        printf("%hhX ", buffer[i]);
    }
    printf("\n");
}

 
typedef struct virus {
    unsigned short length;
    char name[16];
    char signature[];
} virus;

void print_virus(virus* v){
    printf("Virus name: %s\n", v->name);
    printf("Virus size: %d\n", (v->length-16-2));
    printf("signature: \n");
    print_hex(v->signature, v->length-16-2);
}

int main(int argc, char **argv) {
    FILE* fp = fopen("lab3_signatures_opt_2", "r");
    int endian_f = fgetc(fp); /* 0 for Little Endian, 1 for Big Endian*/
    virus* v;
    char clen[2];
    unsigned short len;
    while(fread(clen, 1, 2, fp) > 0){
        if(endian_f){
            len = clen[0]*0x100 + clen[1];
        }
        else{
            len = clen[1]*0x100 + clen[0];
        }
        v = (virus*)malloc(len);
        v->length = len;
        fread(v->name, 1, v->length - 2, fp);
        print_virus(v);
        free(v);
    }
    return 0;
}