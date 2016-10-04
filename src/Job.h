#ifndef __JOB_H__
#define __JOB_H__

#include "String.h"

typedef enum{
    FOREGROUND, BACKGROUND
}JobMode;

typedef enum{
    TRUNCATE, APPEND
}OutputMode;

typedef struct ArgumentNode_{
    String *argument;
    ArgumentNode_ *next;
}ArgumentNode;

typedef struct ProcessNode_{
    String *program;
    ArgumentNode *argument;
    String *input;
    String *output;
    OutputMode output_mode;
    String *error;
    OutputMode error_mode;
    ProcessNode_ *next;
}ProcessNode;

typedef struct JobNode_{
    ProcessNode *process;
    JobMode mode;
    JobNode_ *next;
}JobNode;

JobNode *parse(String*);
void debugJob(JobNode*);
void freeJob(JobNode*);

#endif
