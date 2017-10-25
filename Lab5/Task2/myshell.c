#include <stdio.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <stdlib.h>
#include "./LineParser.h"
#include <unistd.h>
#include "./JobControl.h"
#include <signal.h>
#include <string.h> 

#define MAX_LENGTH 2048 
typedef void (*sighandler_t)(int);

char dflag = 0;
job* first = 0;
job** job_list = &first;

void sig_handler(int signum){
    printf("This signal is ignored: %s\n", strsignal(signum));
}


void execute(cmdLine* cmd){
    int cpid = fork();
    signal(SIGQUIT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    signal(SIGCHLD, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    if(dflag){
        fprintf(stderr, "Current pid: %i", cpid);
    }
    int status = 0;
    if(cpid==0){
        setpgid(getpid(), getpid());
        if(execvp(cmd->arguments[0], cmd->arguments)==-1){
            perror("Error while executing");
            _exit(EXIT_FAILURE);
        }
    }
    else{
        job=addJob(job_list, cmd->arguments[0]);
        job->pgid=cpid;
        setpgid(cpid, cpid);
        if(cmd->blocking == 1) {
            waitpid(-1, &status, 0);
        }
    }
    
}

int handle_cmd(cmdLine* cmd){
    if(strcmp(cmd->arguments[0], "quit")==0){
        freeJobList(job_list);
        exit(0);
    }
    else if(strcmp(cmd->arguments[0], "cd")==0){
        if(chdir(cmd->arguments[1])==-1){
            perror("Error executing cd");
            _exit(EXIT_FAILURE);
        }
        return 1;
    }
    else if(strcmp(cmd->arguments[0], "jobs")==0){
        printJobs(job_list);
        return 1;
    }
    else{
        return 0;
    }
}
 
int main(int argc, char** argv){
    signal(SIGQUIT, sig_handler);
    signal(SIGTSTP, sig_handler);
    signal(SIGCHLD, sig_handler);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    int i;
    char cmd[MAX_LENGTH];
    char cwd[PATH_MAX];
    struct termios termios;
    tcgetattr(STDIN_FILENO, &termios);
    setpgid(getpid(), getpid());
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

