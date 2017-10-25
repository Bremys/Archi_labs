#include <stdio.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h> 
#include "./LineParser.h"
#include <unistd.h>


#define MAX_LENGTH 2048 
 
char dflag = 0; 


void execute(cmdLine* cmd){
    int cpid = fork();
    if(dflag){
        fprintf(stderr, "Current pid: %i", cpid);
    }
    int status = 0;
    if(cpid==0){
        if(execvp(cmd->arguments[0], cmd->arguments)==-1){
            perror("Error while executing");
            _exit(EXIT_FAILURE);
        }
    }
    else{
        if(cmd->blocking == 1) {
            waitpid(-1, &status, 0);
        }
    }
    
}

int handle_cmd(cmdLine* cmd){
    if(strcmp(cmd->arguments[0], "quit")==0){
        exit(0);
    }
    else if(strcmp(cmd->arguments[0], "cd")==0){
        if(chdir(cmd->arguments[1])==-1){
            perror("Error executing cd");
            _exit(EXIT_FAILURE);
        }
        return 1;
    }
    else{
        return 0;
    }
}
 
int main(int argc, char** argv){
    int i;
    char cmd[MAX_LENGTH];
    char cwd[PATH_MAX];
    
    for(i=0; i<argc; i++){
        if(strcmp(argv[i], "-d")==0){
            dflag=1;
        }
    }
    while(1){
        getcwd(cwd, PATH_MAX);
        printf("Current dir: %s\nEnter command: ", cwd);
        fgets(cmd, MAX_LENGTH, stdin);
        cmdLine* to_exec = parseCmdLines(cmd);
        if(to_exec){
            if(dflag){
                fprintf(stderr, "Currently executing: %s", to_exec->arguments[0]);
            }
            if(!handle_cmd(to_exec)){
                execute(to_exec);
            }
            freeCmdLines(to_exec);
        }
    }
    
}

