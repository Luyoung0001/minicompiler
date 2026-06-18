#ifndef LAB05_STUDENT_H
#define LAB05_STUDENT_H

#include <stdio.h>

typedef struct {
    const char *name;
    int offset;
} StudentVar;

int student_get_var_offset(const StudentVar *vars, int count, const char *name);
void student_gen_expr(FILE *out, const char *kind, int value_or_offset);
void student_gen_stmt(FILE *out, const char *kind, int operand);

#endif
