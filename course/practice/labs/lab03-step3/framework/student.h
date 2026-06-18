#ifndef LAB03_STUDENT_H
#define LAB03_STUDENT_H

typedef enum {
    NODE_UNKNOWN,
    NODE_NUM,
    NODE_IDENT,
    NODE_IF,
    NODE_FUNC,
} StudentNodeType;

typedef struct {
    StudentNodeType type;
    char text[64];
} StudentNode;

StudentNode student_parse_primary(const char *token_kind, const char *text);
StudentNode student_parse_if(const char *cond_name);
StudentNode student_parse_function(const char *func_name);

#endif
