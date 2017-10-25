#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int dflag=0;
char* filename=NULL;
int currentfd=-1;
void* start_of_map = NULL;

/* Function declarations */
void print_debug(char* msg);
void quit();
void toggle_d();
void set_file_name();
int menulen();
void print_menu();
void examine_elf_file();
void unmap(size_t file_size);

void print_debug(char* msg){
    if (dflag){
        fprintf(stderr, "%s", msg);
    }
}

/* Quit should unmap and close any mapped or open files, and "exit normally" */
void quit(){
    print_debug("quitting");
    exit(0);
}

void toggle_d(){
    char* mode[]= {"off", "on"};
    dflag = 1-dflag;
    printf("Debug flag now %s\n", mode[dflag]);
}

void examine_elf_file(){
    set_file_name();
    if (currentfd != -1){
        close(currentfd);
    }
    currentfd = open(filename, O_RDONLY);
    if(currentfd == -1){
        fprintf(stderr, "%s\n", "Error: opening file failed");
    }
    size_t file_size = lseek(currentfd, 0, SEEK_END);
    lseek(currentfd, 0, SEEK_SET);
    start_of_map = mmap(NULL, file_size, PROT_READ, MAP_SHARED, currentfd, 0);
    if (start_of_map==MAP_FAILED){
        fprintf(stderr, "%s\n", "Error: mapping failed");
        unmap(file_size);
        return;
    }
    
    
}

void unmap(size_t file_size){
    if (start_of_map != NULL){
        munmap(currentfd, file_size);
        start_of_map = NULL;
    }
    if(currentfd! = -1){
        if(close(currentfd)==-1){
            fprintf(stderr, "%s\n", "Error: closing file failed");
        }
        currentfd = -1;
        free(filename);
        filename = NULL;
    }
}

void set_file_name(){
    char to_receive[100];
    printf("%s","Insert a filename: ");
    fgets(to_receive, 100, stdin);
    char* curr = to_receive;
    while(*curr!='\n'){
        curr++;
    }
    *curr = '\0';
    char to_print[200];
    snprintf(to_print, 200, "Debug: file name set to %s\n", to_receive);
    print_debug(to_print);
    if(filename!=NULL){
        free(filename);
    }
    filename = strdup(to_receive);
}

typedef struct menu_item{
    void (*fun)();
    const char* name;
} menu_item;

menu_item menu[] = { {toggle_d, "Toggle Debug Mode"}, {examine_elf_file, "Examine ELF File"}, {quit, "Quit"}, {NULL, NULL} };

int menulen(){
    int i=0;
    while(menu[i].name!=NULL){
        i++;
    }
    return i;
}

void print_menu(){
    int i;
    printf("Choose action: \n");
    for(i=0; i<menulen(); i++){
        printf("%i-%s\n", i, menu[i].name);
    }
}

int main(int argc, char **argv){
    print_menu();
    int option = fgetc(stdin)-'0';
    while(option>=0 && option <menulen()){
        fgetc(stdin);
        (*menu[option].fun)();
        print_menu();
        option = fgetc(stdin)-'0';
    }
    return 0;
}