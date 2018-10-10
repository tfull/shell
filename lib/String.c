#include "String.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char String_at(String *s, int i){
    if(i < 0 || i >= s->size){
        return '\0';
    }else{
        return s->data[i];
    }
}

char String_charAt(String *s, int i){
    if(i < 0 || i >= s->size){
        return '\0';
    }else{
        return s->data[i];
    }
}

void String_pushBack(String *s, char c){
    if(s->size == s->capacity){
        char *tmp;
        int i;

        s->capacity *= 2;
        tmp = (char*)malloc(s->capacity);
        for(i = 0; i < s->size; i++){
            tmp[i] = s->data[i];
        }
        free(s->data);
        s->data = tmp;
    }

    s->data[s->size] = c;
    s->size += 1;
}

void String_append(String *s, String *t){
    int capacity, size;
    int slen = String_size(s);
    int tlen = String_size(t);
    int i;

    capacity = s->capacity;
    size = slen + tlen;
    while(capacity < size){
        capacity *= 2;
        if(capacity < 0){
            return;
        }
    }
    if(capacity != s->capacity){
        char *data = (char*)malloc(sizeof(char) * capacity);

        for(i = 0; i < slen; i++){
            data[i] = String_at(s, i);
        }
        for(i = 0; i < tlen; i++){
            data[i + slen] = String_at(t, i);
        }
        free(s->data);
        s->data = data;
        s->size += tlen;
    }else{
        for(i = 0; i < tlen; i++){
            s->data[i + slen] = String_at(t, i);
            s->size += tlen;
        }
    }
}

int String_equals(String *s, String *t){
    int i;
    int size;

    size = s->size;

    if(size != t->size){
        return 0;
    }
    for(i = 0; i < size; i++){
        if(s->data[i] != t->data[i]){
            return 0;
        }
    }
    return 1;
}

String *String_new(void){
    String *s = (String*)malloc(sizeof(String));
    s->size = 0;
    s->capacity = 64;
    s->data = (char*)malloc(sizeof(char) * s->capacity);
    return s;
}

String *String_load(char *cs){
    String *s;
    int capacity;
    int i;

    for(i = 0; cs[i] != '\0'; i++){
    }

    for(capacity = 64; capacity <= i; capacity *= 2){
        if(capacity < 0){
            return NULL;
        }
    }

    s = (String*)malloc(sizeof(String));    
    s->capacity = capacity;
    s->size = i;
    s->data = (char*)malloc(sizeof(char) * s->capacity);
    for(i = 0; i < s->size; i++){
        s->data[i] = cs[i];
    }

    return s;
}

String *String_readLine(void){
    int i;
    char c;
    String *s = String_new();

    while((i = getchar()) != EOF){
        c = (char)i;
        String_pushBack(s, c);
        if(c == '\n'){
            break;
        }
    }

    return s;
}

void String_free(String *s){
    free(s->data);
    free(s);
}

int String_size(String *s){
    return s->size;
}

int String_length(String *s){
    return s->size;
}

char *String_toCString(String *s){
    char *cs;
    
    cs = (char *)malloc(sizeof(char) * (s->size + 1));
    for(int i = 0; i < s->size; i++){
        cs[i] = s->data[i];
    }
    cs[s->size] = '\0';

    return cs;
}

void String_copy(String *s, char *cs){
    int i;

    for(i = 0; i < s->size; i++){
        cs[i] = s->data[i];
    }

    cs[i] = '\0';
}

void String_chomp(String *s){
    if(s->size > 0 && s->data[s->size - 1] == '\n'){
        s->size -= 1;
    }
}

void String_stripL(String *s){
    int b = 0;
    int i;

    while(b <= s->size && isspace(s->data[b])){
        b++;
    }

    for(i = 0; i < s->size - b; i++){
        s->data[i] = s->data[i + b];
    }

    s->size -= b;
}

void String_stripR(String *s){
    while(s->size > 0 && isspace(s->data[s->size - 1])){
        s->size--;
    }
}
