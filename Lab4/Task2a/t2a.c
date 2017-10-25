#include "util.h"

#define SYS_EXIT 1
#define SYS_READ  3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_GETDENTS 141

#define BUFFER_SIZE 8192

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define RO 0
#define WO 1
#define RW 2
#define CREATE 64

typedef struct ent {
    unsigned long  d_ino;     /* Inode number */
    unsigned long  d_off;     /* Offset to next linux_dirent */
    unsigned short d_reclen;  /* Length of this linux_dirent */
    char           d_name[];  /* Filename (null-terminated) */
 } ent;


extern int system_call();

void debug_print(char d_flag, char* msg, char* more_info, int call_code, int call_res){
    if(d_flag){
        system_call(SYS_WRITE, STDERR, msg, strlen(msg));
        system_call(SYS_WRITE, STDERR, more_info, strlen(more_info));
        system_call(SYS_WRITE, STDERR, "DEBUG: Sys call code: ", strlen("DEBUG: Sys call code: "));
        system_call(SYS_WRITE, STDERR, itoa(call_code), strlen(itoa(call_code)));
        system_call(SYS_WRITE, STDERR, "\n", 1);
        system_call(SYS_WRITE, STDERR, "DEBUG: Sys call res: ", strlen("DEBUG: Sys call res: "));
        system_call(SYS_WRITE, STDERR, itoa(call_res), strlen(itoa(call_res)));
        system_call(SYS_WRITE, STDERR, "\n", 1);
    }
}

void error_check(int result){
    if(result<0){
        system_call(SYS_EXIT, 0x55);
    }
}


int main (int argc , char* argv[], char* envp[])
{
    char d_flag = 0;
    char buf[BUFFER_SIZE];
    int fd;
    int i;
    for(i=0; i<argc; i++){
        if(strcmp(argv[i], "-d")==0){
            d_flag=1;
        }
    }
    fd = system_call(SYS_OPEN, ".", RO, 0777);
    debug_print(d_flag, "DEBUG: opening current dir \n", "", SYS_OPEN, fd);
    error_check(fd);
    int count = system_call(SYS_GETDENTS, fd, buf, BUFFER_SIZE);
    debug_print(d_flag, "DEBUG: reading file entries \n", "", SYS_GETDENTS, count);
    error_check(count);
    
    ent* entp = (ent*) buf;
    
    while(entp<buf+count){
        int countW = system_call(SYS_WRITE, STDOUT, entp->d_name, strlen(entp->d_name));
        system_call(SYS_WRITE, STDERR, "\n", 1);
        debug_print(d_flag, "DEBUG: printing dir name: \n", entp->d_name, SYS_WRITE, countW);
        error_check(countW);
        debug_print(d_flag, "DEBUG: printing dirent length: \n", itoa(entp->d_reclen), SYS_WRITE, countW);
        entp = (ent*) (((char*)entp)+entp->d_reclen);
    }
    return 0;
}
