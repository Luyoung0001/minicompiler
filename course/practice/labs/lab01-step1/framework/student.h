#ifndef LAB01_STUDENT_H
#define LAB01_STUDENT_H

#include <stdio.h>

typedef struct {
    const char *input_file;
    const char *output_file;
    int verbose;
} StudentConfig;

int student_parse_args(int argc, char **argv, StudentConfig *cfg);
char *student_read_file(const char *filename);
int student_emit_stub_program(FILE *out);

#endif
