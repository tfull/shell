#include "String.h"
#include "Job.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute(JobNode *job){
    for(; job != NULL; job = job->next){
        ProcessNode *process;
        pid_t *children_pid;
        int last_fd = -1;
        int i_process, n_process;

        n_process = getProcessNumber(job->process);
        if(n_process == 0){
            break;
        }
        children_pid = (int*)malloc(sizeof(pid_t) * n_process);

        for(i_process = 0, process = job->process; process != NULL; i_process++, process = process->next){
            pid_t pid;
            int pipefd[2];

            pipe(pipefd);
            pid = fork();

            if(pid == 0){
                char *program_name;
                char **arguments;

                if(last_fd != -1){
                    close(0);
                    dup2(last_fd, 0);
                }
                if(process->next != NULL){
                    close(1);
                    dup2(pipefd[1], 1);
                }

                program_name = String_toCString(process->program);
                arguments = generateArguments(process);
                if(execve(program_name, arguments, NULL) < 0){
                    exit(1);
                }
            }else{
                if(last_fd != -1){
                    close(last_fd);
                }
                if(process->next != NULL){
                    close(pipefd[1]);
                }
                last_fd = pipefd[0];
                children_pid[i_process] = pid;
            }
        }

        for(i_process = 0; i_process < n_process; i_process++){
            int status;
            waitpid(children_pid[i_process], &status, 0);
        }
        free(children_pid);
    }
}

int main(int argc, char *argv[]){
    while(1){
        String *input;
        JobNode *job;

        printf("shell$ ");
        input = String_readLine();

        if(String_size(input) == 0){
            printf("\n");
            break;
        }

        String_chomp(input);

        job = parse(input);
        debugJob(job);

        execute(job);

        freeJob(job);

        String_free(input);
    }
    return 0;
}
