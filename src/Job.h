#ifndef __JOB_H__
#define __JOB_H__

#include "String.h"

typedef struct ProcessNode_{
    String *command;
    String **arguments;
    String *input;
    String *output;
    String *error;
    ProcessNode_ *next;
}ProcessNode;

typedef struct JobNode_{
    ProcessNode *process;
    JobNode_ *next;
}JobNode;

JobNode *parse(String*);

#endif
