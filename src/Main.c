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
    }
}

int main(int argc, char *argv[]){
    while(1){
        String *input;
        JobNode *job;
        pid_t pid;

        printf("shell$ ");
        input = String_readLine();

        if(String_size(input) == 0){
            printf("\n");
            break;
        }

        String_chomp(input);

        job = parse(input);
        debugJob(job);

        pid = fork();

        if(pid == 0){
            char *program_name;
            char **arguments;
            int i;
            program_name = String_toCString(job->process->program);
            arguments = generateArguments(job->process);
            for(i = 0; arguments[i] != NULL; i++){
                printf("%s\n", arguments[i]);
            }
            if(execve(program_name, arguments, NULL) < 0){
                exit(1);
            }
        }else{
            int status = 0;
            waitpid(pid, &status, 0);
        }

        freeJob(job);

        String_free(input);
    }
    return 0;
}
