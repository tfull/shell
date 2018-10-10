#include "Job.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum{
    TS_PLAIN, TS_ON, TS_OUT, TS_ERROR // TS_STRING
}TokenizerState;

typedef enum{
    TT_STRING, TT_IN, TT_OUT, TT_OUT_APPEND, TT_ERROR, TT_ERROR_APPEND, TT_PIPE, TT_BACK, TT_END
}TokenType;

typedef struct TokenNode_{
    TokenType type;
    String *string;
    struct TokenNode_ *next;
}TokenNode;

typedef enum{
    PS_BEGIN, PS_PROGRAM, PS_ARGUMENT, PS_IN, PS_OUT, PS_ERROR, PS_OPTION, PS_END
}ParserState;

TokenNode *generateToken(TokenType tt, String *s){
    TokenNode *tn = (TokenNode*)malloc(sizeof(TokenNode));
    tn->type = tt;
    tn->string = s;
    tn->next = NULL;
    return tn;
}

void pushToken(TokenNode **t0, TokenNode *t1){
    if(*t0 == NULL){
        *t0 = t1;
    }else{
        TokenNode *tn = *t0;
        while(tn->next != NULL){
            tn = tn->next;
        }
        tn->next = t1;
    }
}

TokenNode *tokenize(String *input){
    TokenNode *node = NULL;
    String *buffer;
    String *s_one;
    String *s_two;
    int input_size;
    int i;
    TokenizerState state = TS_PLAIN;

    s_one = String_load((char*)"1");
    s_two = String_load((char*)"2");

    buffer = NULL;
    input_size = String_size(input);

    for(i = 0; i < input_size; i++){
        char c = String_at(input, i);

        if(state == TS_PLAIN){
            if(isspace(c)){
            }else  if(c == '|'){
                pushToken(&node, generateToken(TT_PIPE, NULL));
            }else if(c == '&'){
                pushToken(&node, generateToken(TT_BACK, NULL));
            }else if(c == '<'){
                pushToken(&node, generateToken(TT_IN, NULL));
            }else if(c == '>'){
                state = TS_OUT;
            }else if(c == ';'){
                pushToken(&node, generateToken(TT_END, NULL));
            }else{
                buffer = String_new();
                String_pushBack(buffer, c);
                state = TS_ON;
            }
        }else if(state == TS_ON){
            if(c == '|'){
                pushToken(&node, generateToken(TT_STRING, buffer));
                pushToken(&node, generateToken(TT_PIPE, NULL));
                buffer = NULL;
                state = TS_PLAIN;
            }else if(c == '&'){
                pushToken(&node, generateToken(TT_STRING, buffer));
                pushToken(&node, generateToken(TT_BACK, NULL));
                buffer = NULL;
                state = TS_PLAIN;
            }else if(c == '<'){
                pushToken(&node, generateToken(TT_STRING, buffer));
                pushToken(&node, generateToken(TT_IN, NULL));
                buffer = NULL;
                state = TS_PLAIN;
            }else if(c == '>'){
                if(String_equals(buffer, s_one)){
                    state = TS_OUT;
                    String_free(buffer);
                    buffer = NULL;
                }else if(String_equals(buffer, s_two)){
                    state = TS_ERROR;
                    String_free(buffer);
                    buffer = NULL;
                }else{
                    pushToken(&node, generateToken(TT_STRING, buffer));
                    buffer = NULL;
                    state = TS_OUT;
                }
            }else if(c == ';'){
                pushToken(&node, generateToken(TT_STRING, buffer));
                pushToken(&node, generateToken(TT_END, NULL));
                buffer = NULL;
                state = TS_PLAIN;
            }else if(isspace(c)){
                pushToken(&node, generateToken(TT_STRING, buffer));
                buffer = NULL;
                state = TS_PLAIN;
            }else{
                String_pushBack(buffer, c);
            }
        }else if(state == TS_OUT){
            if(c == '>'){
                pushToken(&node, generateToken(TT_OUT_APPEND, NULL));
                state = TS_PLAIN;
            }else{
                pushToken(&node, generateToken(TT_OUT, NULL));
                state = TS_PLAIN;
                if(c == '|'){
                    pushToken(&node, generateToken(TT_PIPE, NULL));
                }else if(c == '&'){
                    pushToken(&node, generateToken(TT_BACK, NULL));
                }else if(c == '<'){
                    pushToken(&node, generateToken(TT_IN, NULL));
                }else if(c == ';'){
                    pushToken(&node, generateToken(TT_END, NULL));
                }else if(! isspace(c)){
                    buffer = String_new();
                    String_pushBack(buffer, c);
                    state = TS_ON;
                }
            }
        }else if(state == TS_ERROR){
            if(c == '>'){
                pushToken(&node, generateToken(TT_ERROR_APPEND, NULL));
                state = TS_PLAIN;
            }else{
                pushToken(&node, generateToken(TT_ERROR, NULL));
                state = TS_PLAIN;
                if(c == '|'){
                    pushToken(&node, generateToken(TT_PIPE, NULL));
                }else if(c == '&'){
                    pushToken(&node, generateToken(TT_BACK, NULL));
                }else if(c == '<'){
                    pushToken(&node, generateToken(TT_IN, NULL));
                }else if(c == ';'){
                    pushToken(&node, generateToken(TT_END, NULL));
                }else if(! isspace(c)){
                    buffer = String_new();
                    String_pushBack(buffer, c);
                    state = TS_ON;
                }
            }
        }
    }

    if(state == TS_ON){
        pushToken(&node, generateToken(TT_STRING, buffer));
    }else if(state == TS_OUT){
        pushToken(&node, generateToken(TT_OUT, NULL));
    }else if(state == TS_ERROR){
        pushToken(&node, generateToken(TT_ERROR, NULL));
    }

    String_free(s_one);
    String_free(s_two);

    return node;
}

void debugTokens(TokenNode *token){
    FILE *fp = stderr;
    int index = 0;

    while(token != NULL){
        index++;
        printf("=%d=\n", index);
        if(token->type == TT_STRING){
            char *s = String_toCString(token->string);
            fprintf(fp, "STRING: %s\n", s);
            free(s);
        }else if(token->type == TT_IN){
            fprintf(fp, "IN\n");
        }else if(token->type == TT_OUT){
            fprintf(fp, "OUT\n");
        }else if(token->type == TT_OUT_APPEND){
            fprintf(fp, "OUT_APPEND\n");
        }else if(token->type == TT_ERROR){
            fprintf(fp, "ERROR\n");
        }else if(token->type == TT_ERROR_APPEND){
            fprintf(fp, "ERROR_APPEND\n");
        }else if(token->type == TT_PIPE){
            fprintf(fp, "PIPE\n");
        }else{
            fprintf(fp, "BACK\n");
        }
        token = token->next;
    }
}

void debugJob(JobNode *job){
    int job_count;
    int process_count;
    ProcessNode *process;
    FILE *fp = stderr;

    for(job_count = 1; job != NULL; job = job->next, job_count++){
        fprintf(fp, "job %d: ", job_count);
        if(job->mode == FOREGROUND){
            fprintf(fp, "mode = foreground\n");
        }else{
            fprintf(fp, "mode = background\n");
        }
        for(process_count = 1, process = job->process; process != NULL; process = process->next, process_count++){
            fprintf(fp, "  process %d\n", process_count);
            if(process->program != NULL){
                char *s = String_toCString(process->program);
                fprintf(fp, "    program: %s\n", s);
                free(s);
            }
            if(process->argument != NULL){
                ArgumentNode *an = process->argument;
                int argument_count = 1;
                
                for(; an != NULL; an = an->next, argument_count++){
                    char *s = String_toCString(an->argument);
                    fprintf(fp, "    argument %d: %s\n", argument_count, s);
                    free(s);
                }
            }
            if(process->input != NULL){
                char *s = String_toCString(process->input);
                fprintf(fp, "    input: %s\n", s);
                free(s);
            }
            if(process->output != NULL){
                char *s = String_toCString(process->output);
                if(process->output_mode == TRUNCATE){
                    fprintf(fp, "    output (truncate): %s\n", s);
                }else{
                    fprintf(fp, "    output (append): %s\n", s);
                }
                free(s);
            }
            if(process->error != NULL){
                char *s = String_toCString(process->error);
                if(process->error_mode == TRUNCATE){
                    fprintf(fp, "    error (truncate): %s\n", s);
                }else{
                    fprintf(fp, "    error (append): %s\n", s);
                }
                free(s);
            }
        }
    }
}

void freeToken(TokenNode *token){
    while(token != NULL){
        TokenNode *trash = token;
        token = token->next;
        if(trash->string != NULL){
            String_free(trash->string);
        }
        free(trash);
    }
}

void freeArguments(ArgumentNode *argument){
    while(argument != NULL){
        ArgumentNode *trash;
        trash = argument;
        argument = argument->next;
        String_free(trash->argument);
        free(trash);
    }
}

void freeJob(JobNode *job){
    while(job != NULL){
        ProcessNode *p = job->process;
        JobNode *trash_job = job;

        job = job->next;

        while(p != NULL){
            ProcessNode *trash_p = p;
            p = p->next;

            if(trash_p->program != NULL){
                String_free(trash_p->program);
            }
            freeArguments(trash_p->argument);
            if(trash_p->input){
                String_free(trash_p->input);
            }
            if(trash_p->output){
                String_free(trash_p->output);
            }
            if(trash_p->error){
                String_free(trash_p->error);
            }

            free(trash_p);
        }

        free(trash_job);
    }
}

void pushJob(JobNode *j0, JobNode *j1){
    JobNode *jn = j0;
    while(jn->next != NULL){
        jn = jn->next;
    }
    jn->next = j1;
}

void pushArgument(ProcessNode *process, String *s){
    ArgumentNode *arg = (ArgumentNode*)malloc(sizeof(ArgumentNode));

    arg->argument = s;
    arg->next = NULL;

    if(process->argument == NULL){
        process->argument = arg;
    }else{
        ArgumentNode *a = process->argument;
        while(a->next != NULL){
            a = a->next;
        }
        a->next = arg;
    }
}

void pushProcess(JobNode *job, ProcessNode *process){
    if(job->process == NULL){
        job->process = process;
    }else{
        ProcessNode *p = job->process;
        while(p->next != NULL){
            p = p->next;
        }
        p->next = process;
    }
}

JobNode *createEmptyJob(){
    JobNode *jn = (JobNode*)malloc(sizeof(JobNode));
    jn->process = NULL;
    jn->mode = FOREGROUND;
    return jn;
}

ProcessNode *createEmptyProcess(){
    ProcessNode *p = (ProcessNode*)malloc(sizeof(ProcessNode));
    p->program = NULL;
    p->argument = NULL;
    p->input = NULL;
    p->output = NULL;
    p->output_mode = TRUNCATE;
    p->error = NULL;
    p->error_mode = TRUNCATE;
    p->next = NULL;
    return p;
}

JobNode *parse(String *input){
    TokenNode *token = tokenize(input);
    JobNode *job;
    JobNode *jn;
    ProcessNode *pn;
    TokenNode *tn;
    ParserState state = PS_BEGIN;
    FILE *fp = stderr;
    char *error_prompt = (char*)"shell error";
    int invalid = 0;

    jn = createEmptyJob();
    job = jn;

    for(tn = token; tn != NULL; tn = tn->next){
        TokenType tt = tn->type;
        if(state == PS_BEGIN){
            if(tt == TT_STRING){
                pn = createEmptyProcess();
                pn->program = tn->string;
                pushProcess(jn, pn);
                tn->string = NULL;
                state = PS_ARGUMENT;
            }else if(tt == TT_END){
                pushJob(job, jn);
                jn = createEmptyJob();
            }else{
                fprintf(fp, "%s: no program name\n", error_prompt);
                freeToken(token);
                freeJob(job);
                return NULL;
            }
        }else if(state == PS_PROGRAM){
            if(tt == TT_STRING){
                pn = createEmptyProcess();
                pn->program = tn->string;
                pushProcess(jn, pn);
                tn->string = NULL;
                state = PS_ARGUMENT;
            }else{
                fprintf(fp, "%s: no program name\n", error_prompt);
                freeToken(token);
                freeJob(job);
                return NULL;
            }
        }else if(state == PS_ARGUMENT){
            if(tt == TT_STRING){
                pushArgument(pn, tn->string);
                tn->string = NULL;
            }else if(tt == TT_IN){
                state = PS_IN;
            }else if(tt == TT_OUT){
                pn->output_mode = TRUNCATE;
                state = PS_OUT;
            }else if(tt == TT_OUT_APPEND){
                pn->output_mode = APPEND;
                state = PS_OUT;
            }else if(tt == TT_ERROR){
                pn->error_mode = TRUNCATE;
                state = PS_ERROR;
            }else if(tt == TT_ERROR_APPEND){
                pn->error_mode = APPEND;
                state = PS_ERROR;
            }else if(tt == TT_PIPE){
                state = PS_PROGRAM;
            }else if(tt == TT_BACK){
                jn->mode = BACKGROUND;
                state = PS_END;
            }else{
                pn = NULL;
                jn = createEmptyJob();
                pushJob(job, jn);
                state = PS_BEGIN;
            }
        }else if(state == PS_IN){
            if(tt == TT_STRING){
                pn->input = tn->string;
                tn->string = NULL;
                state = PS_OPTION;
            }else{
                fprintf(fp, "%s: no input file\n", error_prompt);
                freeToken(token);
                freeJob(job);
                return NULL;
            }
        }else if(state == PS_OUT){
            if(tt == TT_STRING){
                pn->output = tn->string;
                tn->string = NULL;
                state = PS_OPTION;
            }else{
                fprintf(fp, "%s: no output file\n", error_prompt);
                freeToken(token);
                freeJob(job);
                return NULL;
            }
        }else if(state == PS_ERROR){
            if(tt == TT_STRING){
                pn->error = tn->string;
                tn->string = NULL;
                state = PS_OPTION;
            }else{
                fprintf(fp, "%s: no output file\n", error_prompt);
                freeToken(token);
                freeJob(job);
                return NULL;
            }
        }else if(state == PS_OPTION){
            if(tt == TT_IN){
                state = PS_IN;
            }else if(tt == TT_OUT){
                pn->output_mode = TRUNCATE;
                state = PS_OUT;
            }else if(tt == TT_OUT_APPEND){
                pn->output_mode = APPEND;
                state = PS_OUT;
            }else if(tt == TT_ERROR){
                pn->error_mode = TRUNCATE;
                state = PS_ERROR;
            }else if(tt == TT_ERROR_APPEND){
                pn->error_mode = APPEND;
                state = PS_ERROR;
            }else if(tt == TT_PIPE){
                state = PS_PROGRAM;
            }else if(tt == TT_BACK){
                jn->mode = BACKGROUND;
                state = PS_END;
            }else if(tt == TT_END){
                pn = NULL;
                jn = createEmptyJob();
                pushJob(job, jn);
                state = PS_BEGIN;
            }
        }else{
            if(tt == TT_END){
                pn = NULL;
                jn = createEmptyJob();
                pushJob(job, jn);
                state = PS_BEGIN;
            }else{
                fprintf(fp, "%s: extra thing\n", error_prompt);
                freeToken(token);
                freeJob(job);
                return NULL;
            }
        }
    }

    if(state == PS_PROGRAM){
        fprintf(fp, "%s: no program name\n", error_prompt);
        invalid = 1;
    }else if(state == PS_IN){
        fprintf(fp, "%s: no input file\n", error_prompt);
        invalid = 1;
    }else if(state == PS_OUT || state == PS_ERROR){
        fprintf(fp, "%s: no output file\n", error_prompt);
        invalid = 1;
    }

    freeToken(token);

    if(invalid){
        freeJob(job);
        return NULL;
    }else{
        return job;
    }
}

int lengthOfArgumentNode(ArgumentNode *node){
    int count;
    for(count = 0; node != NULL; count++, node = node->next);
    return count;
}

char **generateArguments(ProcessNode *pnode){
    int length, index;
    char **arguments;
    ArgumentNode *anode;

    anode = pnode->argument;

    length = lengthOfArgumentNode(anode);
    arguments = (char**)malloc(sizeof(void*) * (length + 2));
    arguments[0] = String_toCString(pnode->program);
    arguments[length + 1] = NULL;

    for(index = 1; index <= length; index++, anode = anode->next){
        arguments[index] = String_toCString(anode->argument);
    }

    return arguments;
}
