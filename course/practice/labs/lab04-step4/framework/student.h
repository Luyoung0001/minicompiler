#ifndef LAB04_STUDENT_H
#define LAB04_STUDENT_H

typedef struct StudentSymbol {
    char name[64];
    struct StudentSymbol *next;
} StudentSymbol;

typedef struct {
    StudentSymbol *head;
} StudentScope;

void student_symtab_add(StudentScope *scope, const char *name);
int student_symtab_lookup(StudentScope *scope, const char *name);
int student_semantic_analyze(StudentScope *scope, const char **decls, int decl_count, const char **uses, int use_count);

#endif
