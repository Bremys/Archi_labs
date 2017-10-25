#include "util.h"

#define SYS_READ  3
#define SYS_WRITE 4

#define STDIN 0
#define STDOUT 1
#define STDERR 2

extern int system_call();

void debug_print(char d_flag, char* msg, int call_code, int call_res){
    if(d_flag){
        system_call(SYS_WRITE, STDERR, msg, strlen(msg));
        system_call(SYS_WRITE, STDERR, "DEBUG: Sys call code: ", strlen("DEBUG: Sys call code: "));
        system_call(SYS_WRITE, STDERR, itoa(call_code), strlen(itoa(call_code)));
        system_call(SYS_WRITE, STDERR, "\n", 1);
        system_call(SYS_WRITE, STDERR, "DEBUG: Sys call res: ", strlen("DEBUG: Sys call res: "));
        system_call(SYS_WRITE, STDERR, itoa(call_res), strlen(itoa(call_res)));
        system_call(SYS_WRITE, STDERR, "\n", 1);
    }
}


int main (int argc , char* argv[], char* envp[])
{
    char d_flag = 0;
    int i;
    for(i=0; i<argc; i++){
        if(strcmp(argv[i], "-d")==0){
            d_flag=1;
        }
    }
    char curr_c;
    int countR = system_call(SYS_READ, STDIN, &curr_c, 1);
    debug_print(d_flag, "DEBUG: reading from STDIN \n", SYS_READ, countR);
    while(countR>0){
        if(curr_c>='A' && curr_c<='Z'){
            curr_c +=('a'-'A');
        }
        int countW = system_call(SYS_WRITE, STDOUT, &curr_c, 1);
        debug_print(d_flag, "DEBUG: writing to STDOUT \n", SYS_WRITE, countW);
        countR = system_call(SYS_READ, STDIN, &curr_c, 1);
        debug_print(d_flag, "DEBUG: reading from STDIN \n", SYS_READ, countR);
    }
    return 0;
}
