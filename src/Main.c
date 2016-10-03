#include "String.h"
#include "Job.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    while(1){
        String *input;
        printf("shell$ ");
        input = String_readLine();
        String_chomp(input);

        parse(input);

        String_free(input);
    }
    return 0;
}
