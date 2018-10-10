#ifndef __STRING_H__
#define __STRING_H__

typedef struct{
    char *data;
    int size;
    int capacity;
}String;

void String_append(String*, String*);
int String_equals(String*, String*);
char String_at(String*, int);
char String_charAt(String*, int);
void String_chomp(String*);
void String_copy(String*, char*);
void String_free(String*);
int String_length(String*);
String *String_load(char*);
String *String_new(void);
void String_pushBack(String*, char);
String *String_readLine(void);
int String_size(String*);
void String_stripL(String*);
void String_stripR(String*);
char *String_toCString(String*);

#endif
