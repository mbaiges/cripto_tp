#ifndef _ARGUMENTS_H
#define _ARGUMENTS_H

#include <stdint.h>

typedef enum {DISTRIBUTE=0, RECOVER} op_typ; 

typedef struct args_st {
    op_typ op;
    char * secret_filename;
    uint8_t k;
    char * shades_directory;
} args_st;

/**
Parse arguments 
@argc: number of arguments
@argv: variable arguments
@args: struct to save parsed arguments
@output: returns 0 if no error
**/ 
int parse_arguments(int argc, char ** argv, args_st * args);

#endif