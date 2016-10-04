#include "String.h"
#include "Job.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void execute(JobNode *job){
    for(; job != NULL; job = job->next){
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
        freeJob(job);

        String_free(input);
    }
    return 0;
}
