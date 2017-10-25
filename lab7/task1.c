#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int dflag=0;
char* filename=NULL;
char unit_size=1;
void* data_ptr=NULL;

void print_debug(char* msg){
    if (dflag){
        fprintf(stderr, "%s", msg);
    }
}

void quit(){
    print_debug("quitting");
    exit(0);
}

void toggle_d(){
    char* mode[]= {"off", "on"};
    dflag = 1-dflag;
    printf("Debug flag now %s\n", mode[dflag]);
}

void set_unit_size(){
    char input;
    printf("%s","Insert a unit size: ");
    input = fgetc(stdin)-'0';
    fgetc(stdin);
    char to_print[30];
    snprintf(to_print, 30, "Debug: set size to %d\n", (int)input);
    print_debug(to_print);
    if(input != 1 && input != 2 && input != 4){
        printf("%s","Illegal input\n");
        return;
    }

    unit_size = input;
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


void print_hex_char(unsigned char* data, int length){
    int i;
    for(i=0; i<length; ++i){
        printf("%02hx ", data[i]);
    }
    printf("\n");
}



void print_hex_short(unsigned short* data, int length){
    int i;
    for(i=0; i<length; ++i){
        printf("%04hx ", data[i]);
    }
    printf("\n");
}


void print_hex_int(int* data, int length){
    int i;
    for(i=0; i<length; ++i){
        printf("%x ", data[i]);
    }
    printf("\n");
}

void print_hex(void* data, int length){
    if(unit_size==1){
        print_hex_char((char*)data, length);
    }
    else if(unit_size==2){
        print_hex_short((short*)data, length);
    }
    else if(unit_size==4){
        print_hex_int((int*)data, length);
    }
}



void print_dec_char(unsigned char* data, int length){
    int i;
    for(i=0; i<length; ++i){
        printf("%d ", data[i]);
    }
    printf("\n");
}



void print_dec_short(unsigned short* data, int length){
    int i;
    for(i=0; i<length; ++i){
        printf("%d ", data[i]);
    }
    printf("\n");
}


void print_dec_int(int* data, int length){
    int i;
    for(i=0; i<length; ++i){
        printf("%d ", data[i]);
    }
    printf("\n");
}

void print_dec(void* data, int length){
    if(unit_size==1){
        print_dec_char((char*)data, length);
    }
    else if(unit_size==2){
        print_dec_short((short*)data, length);
    }
    else if(unit_size==4){
        print_dec_int((int*)data, length);
    }
}

void file_display (){
    if(filename==NULL){
        fprintf(stderr, "%s\n", "Error: file name is NULL");
        return;
    }
    int fd = open(filename, O_RDONLY);
    if (fd<0){
        fprintf(stderr, "%s\n", "Error: opening file failed");
        return;
    }
    char to_receive[100];
    int length=0, location=0;
    printf("%s\n","Please enter <location> <length>");
    fgets(to_receive, 100, stdin);
    sscanf( to_receive, "%x %d", &location, &length );
    char to_print[200];
    snprintf(to_print, 200, "Debug: location is 0x%x and length is %d\n", location, length);
    print_debug(to_print);
    void* data =malloc(unit_size*length);
    lseek(fd, location, SEEK_SET);
    read(fd, data, unit_size*length);
    close(fd);
    print_hex(data, length);
    print_dec(data, length);
    free(data);
}


void load_to_mem (){
    if(filename==NULL){
        fprintf(stderr, "%s\n", "Error: file name is NULL");
        return;
    }
    int fd = open(filename, O_RDONLY);
    if (fd<0){
        fprintf(stderr, "%s\n", "Error: opening file failed");
        return;
    }
    char to_receive[100];
    int length=0, location=0;
    printf("%s\n","Please enter <location> <length>");
    fgets(to_receive, 100, stdin);
    sscanf( to_receive, "%x %d", &location, &length );
    if(data_ptr!=NULL){
        free(data_ptr);
    }
    data_ptr=malloc(length);
    char to_print[200];
    snprintf(to_print, 200, "Debug: data_ptr is %x, location is 0x%x and length is %d\n", (int)data_ptr, location, length);
    print_debug(to_print);
    lseek(fd, location, SEEK_SET);
    read(fd, data_ptr, length);
    close(fd);
}

void save_to_file (){
    if(filename==NULL){
        fprintf(stderr, "%s\n", "Error: file name is NULL");
        return;
    }
    int fd = open(filename, O_WRONLY);
    if (fd<0){
        fprintf(stderr, "%s\n", "Error: opening file failed");
        return;
    }
    char to_receive[100];
    int length=0, location=0;
    void* mem_address = NULL;
    printf("%s\n","Please enter <mem_address> <location> <length>");
    fgets(to_receive, 100, stdin);
    sscanf( to_receive, "%x %x %d",(int *) &mem_address, &location, &length );
    if(mem_address==0){
        mem_address=data_ptr;
    }
    char to_print[200];
    snprintf(to_print, 200, "Debug: mem_address is %x, location is 0x%x and length is %d\n", (int)data_ptr, location, length);
    print_debug(to_print);
    int size = lseek(fd, 0, SEEK_END);
    if(size<location){
        fprintf(stderr, "%s\n", "Error: location bigger than file size");
        return;
    }
    lseek(fd, location, SEEK_SET);
    write(fd, mem_address, length);
    close(fd);
}

void file_modify(){
    if(filename==NULL){
        fprintf(stderr, "%s\n", "Error: file name is NULL");
        return;
    }
    int fd = open(filename, O_WRONLY);
    if (fd<0){
        fprintf(stderr, "%s\n", "Error: opening file failed");
        return;
    }
    char to_receive[100];
    int val=0, location=0;
    printf("%s\n","Please enter <location> <val>");
    fgets(to_receive, 100, stdin);
    sscanf( to_receive, "%x %x",&location, &val );
    char to_print[200];
    snprintf(to_print, 200, "Debug: val is %x and location is 0x%x\n", val, location);
    print_debug(to_print);
    int size = lseek(fd, 0, SEEK_END);
    if(size<location){
        fprintf(stderr, "%s\n", "Error: location bigger than file size");
        return;
    }
    lseek(fd, location, SEEK_SET);
    write(fd, (char*)&val, unit_size);
    close(fd);
}

typedef struct menu_item{
    void (*fun)();
    const char* name;
} menu_item;

menu_item menu[] = { {toggle_d, "Toggle Debug Mode"}, {set_file_name, "Set File Name"}, {set_unit_size, "Set Unit Size"}, {file_display, "File Display"},{load_to_mem, "Load Into Memory"}, {save_to_file, "Save To File"}, {file_modify, "File Modify"}, {quit, "Quit"}, {NULL, NULL} };


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