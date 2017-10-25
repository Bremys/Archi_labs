#include "util.h"

#define SYS_READ  3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6

#define STDIN 0
#define STDOUT 1
#define STDERR 2



extern int system_call();

void debug_print(char d_flag, char* msg, char* file_name, int call_code, int call_res){
    if(d_flag){
        system_call(SYS_WRITE, STDERR, msg, strlen(msg));
        system_call(SYS_WRITE, STDERR, file_name, strlen(file_name));
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
    int in_file = STDIN;
    int out_file = STDOUT;
    char* in_name = "STDIN";
    char* out_name = "STDOUT";
    for(i=0; i<argc; i++){
        if(strcmp(argv[i], "-d")==0){
            d_flag=1;
        }
    }
    for(i=0; i<argc; i++){
        if(strcmp(argv[i], "-i")==0){
            in_name=argv[i+1];
            in_file=system_call(SYS_OPEN, in_name, 0, 0777);
        }
        if(strcmp(argv[i], "-o")==0){
            out_name=argv[i+1];
            out_file=system_call(SYS_OPEN, out_name, 64 | 1, 0777);
        }
    }
    char curr_c;
    int countR = system_call(SYS_READ, in_file, &curr_c, 1);
    debug_print(d_flag, "DEBUG: reading from \n", in_name, SYS_READ, countR);
    while(countR>0){
        if(curr_c>='A' && curr_c<='Z'){
            curr_c +=('a'-'A');
        }
        int countW = system_call(SYS_WRITE, out_file, &curr_c, 1);
        debug_print(d_flag, "DEBUG: writing to \n", out_name, SYS_WRITE, countW);
        countR = system_call(SYS_READ, in_file, &curr_c, 1);
        debug_print(d_flag, "DEBUG: reading from \n", in_name, SYS_READ, countR);
    }
    return 0;
}
