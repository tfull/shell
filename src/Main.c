#include "String.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    while(1){
        String *line;
        char *cs;
        int size;
        printf("shell$ ");
        line = String_readLine();
        String_chomp(line);
        size = String_size(line);
        cs = (char*)malloc((sizeof(char) + 1) * size);
        String_copy(line, cs);
        if(strcmp(cs, "exit") == 0){
            return 0;
        }else{
            printf("%s\n", cs);
        }
    }
    return 0;
}
