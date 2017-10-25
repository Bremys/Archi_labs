
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

void print_detected(virus* v, int i){
    printf("Starting byte: %d\n", i);
    printf("Virus name: %s\n", v->name);
    printf("Virus size: %d\n", v->length-16-2);
}

int memcmp(char* buffer1, char* buffer2, unsigned int size){
    int i;
    for(i=0; i<size; ++i){
        if(buffer1[i]-buffer2[i]){
            return buffer1[i]-buffer2[i];
        }
    }
    return 0;
}

void clean_file(FILE* to_clean,unsigned int start,unsigned int size){
    fseek(to_clean, start, SEEK_SET);
    int i;
    for(i=0; i<size; ++i){
        fputc(0x90, to_clean);
    }
}

void detect_virus(char *buffer, link *virus_list, unsigned int size, FILE* sus){
    int i;
    while(virus_list!=NULL){
        for(i=0; i<size-virus_list->v->length; ++i){
            if(memcmp(buffer+i, virus_list->v->signature, virus_list->v->length-18)==0){
                print_detected(virus_list->v, i);
                clean_file(sus, i, virus_list->v->length-18);
            }
        }
        virus_list=virus_list->next;
    }
}


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
        virus_list = (link*)malloc(1*sizeof(link));
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
    FILE* sig = fopen("lab3_signatures_opt_2", "r");
    FILE* sus = fopen(argv[1], "r+");
    char* buf = (char*)calloc(10000, sizeof(char));
    int size = fread(buf, 1, 10000, sus);
    int big_endian_f = fgetc(sig); /* 0 for Little Endian, 1 for Big Endian*/
    link* virus_list=NULL;
    unsigned short len;
    while(fread(&len, 1, 2, sig) > 0){
        virus* v;
        if(big_endian_f){
            len =  ((len & 0xff) << 8) + ((len & 0xff00) >> 8);
        }
        v = (virus*)malloc(len);
        v->length = len;
        fread(((void*)v)+2, 1, v->length - 2, sig);
        virus_list = list_append(virus_list, v);
    }
    detect_virus(buf, virus_list, size, sus);
    list_free(virus_list);
    free(buf);
    fclose(sus);
    fclose(sig);
    return 0;
}