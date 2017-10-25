
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

typedef struct link link;
 
struct link {
    virus *v;
    link *next;
};

void list_print(link *virus_list){
     while(virus_list!=NULL){
         print_virus(virus_list->v);
         virus_list = virus_list->next;
     }
}
 
link* list_append(link* virus_list, virus* data){
    if(virus_list!=NULL){
        link* head = (link*)malloc(1*sizeof(link));
        head->v = data;
        head->next = virus_list;
        return head;
    }
    else{
        virus_list = (link*)calloc(1*sizeof(link),1);
        virus_list->v = data;
        virus_list->next = NULL;
        return virus_list;
    }
}
 
void list_free(link *virus_list){
    while(virus_list!=NULL){
         link* temp = virus_list;
         virus_list = virus_list->next;
         free(temp->v);
         free(temp);
     }
}

int main(int argc, char **argv) {
    FILE* fp = fopen("lab3_signatures_opt_2", "r");
    int endian_f = fgetc(fp); /* 0 for Little Endian, 1 for Big Endian*/
    link* virus_list=NULL;
    char clen[2];
    unsigned short len;
    while(fread(&len, 1, 2, fp) > 0){
        virus* v;
        if(endian_f){
            len = ((len & 0xff) << 8) + ((len & 0xff00) >> 8);
        }
        v = (virus*)malloc(len);
        v->length = len;
        fread(((void*)v)+2, 1, v->length - 2, fp);   
        virus_list = list_append(virus_list, v);
    }
    list_print(virus_list);
    list_free(virus_list);
    fclose(fp);
    return 0;
}