#include "Job.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum{
    TS_PLAIN, TS_ON, TS_OUT, TS_ERROR // TS_STRING
}TokenizerState;

typedef enum{
    TT_STRING, TT_IN, TT_OUT, TT_OUT_APPEND, TT_ERROR, TT_ERROR_APPEND, TT_PIPE, TT_BACK
}TokenType;

typedef struct TokenNode_{
    TokenType type;
    String *string;
    TokenNode_ *next;
}TokenNode;

typedef enum{
    PS_PLAIN, PS_ARGUMENT, PS_IN, PS_OUT, PS_ERROR, PS_OPTION, PS_END
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
            }else{
                buffer = String_new();
                String_pushBack(buffer, c);
                state = TS_ON;
            }
        }else if(state == TS_ON){
            if(c == '|'){
                pushToken(&node, generateToken(TT_STRING, buffer));
                pushToken(&node, generateToken(TT_PIPE, NULL));
                state = TS_PLAIN;
            }else if(c == '&'){
                pushToken(&node, generateToken(TT_STRING, buffer));
                pushToken(&node, generateToken(TT_BACK, NULL));
                state = TS_PLAIN;
            }else if(c == '<'){
                pushToken(&node, generateToken(TT_STRING, buffer));
                pushToken(&node, generateToken(TT_IN, NULL));
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
    int index = 0;

    while(token != NULL){
        index++;
        printf("=%d=\n", index);
        if(token->type == TT_STRING){
            char *s = String_toCString(token->string);
            printf("STRING: %s\n", s);
            free(s);
        }else if(token->type == TT_IN){
            printf("IN\n");
        }else if(token->type == TT_OUT){
            printf("OUT\n");
        }else if(token->type == TT_OUT_APPEND){
            printf("OUT_APPEND\n");
        }else if(token->type == TT_ERROR){
            printf("ERROR\n");
        }else if(token->type == TT_ERROR_APPEND){
            printf("ERROR_APPEND\n");
        }else if(token->type == TT_PIPE){
            printf("PIPE\n");
        }else{
            printf("BACK\n");
        }
        token = token->next;
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

JobNode *parse(String *input){
    TokenNode *token = tokenize(input);

    debugTokens(token);
    freeToken(token);

    TokenNode *iterator = token;

    ParserState state = PS_PLAIN;


    return NULL;
}
