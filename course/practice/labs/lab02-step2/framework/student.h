#ifndef LAB02_STUDENT_H
#define LAB02_STUDENT_H

typedef enum {
    ST_TOK_EOF,
    ST_TOK_INT,
    ST_TOK_IF,
    ST_TOK_RETURN,
    ST_TOK_IDENT,
    ST_TOK_NUM,
    ST_TOK_UNKNOWN,
} StudentTokenType;

typedef struct {
    const char *source;
    const char *pos;
} StudentLexer;

typedef struct {
    StudentTokenType type;
    char value[64];
} StudentToken;

void student_lexer_init(StudentLexer *lexer, const char *source);
void student_skip_whitespace(StudentLexer *lexer);
StudentToken student_scan_identifier(StudentLexer *lexer);
StudentToken student_scan_number(StudentLexer *lexer);

#endif
