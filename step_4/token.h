/*
 * token.h - Token 定义
 * Step 2: 词法分析器
 */

#ifndef TOKEN_H
#define TOKEN_H

/* Token 类型 */
typedef enum {
    /* 特殊 */
    TOK_EOF = 0,        /* 文件结束 */
    TOK_ERROR,          /* 错误 */

    /* 关键字 */
    TOK_INT,            /* int */
    TOK_CHAR,           /* char */
    TOK_VOID,           /* void */
    TOK_RETURN,         /* return */
    TOK_IF,             /* if */
    TOK_ELSE,           /* else */
    TOK_WHILE,          /* while */
    TOK_FOR,            /* for */

    /* 字面量 */
    TOK_NUM,            /* 数字 */
    TOK_STRING,         /* 字符串 */
    TOK_IDENT,          /* 标识符 */

    /* 运算符 */
    TOK_PLUS,           /* + */
    TOK_MINUS,          /* - */
    TOK_STAR,           /* * */
    TOK_SLASH,          /* / */
    TOK_PERCENT,        /* % */
    TOK_ASSIGN,         /* = */
    TOK_EQ,             /* == */
    TOK_NE,             /* != */
    TOK_LT,             /* < */
    TOK_LE,             /* <= */
    TOK_GT,             /* > */
    TOK_GE,             /* >= */
    TOK_AND,            /* && */
    TOK_OR,             /* || */
    TOK_NOT,            /* ! */
    TOK_AMP,            /* & */
    TOK_PIPE,           /* | */

    /* 分隔符 */
    TOK_LPAREN,         /* ( */
    TOK_RPAREN,         /* ) */
    TOK_LBRACE,         /* { */
    TOK_RBRACE,         /* } */
    TOK_LBRACKET,       /* [ */
    TOK_RBRACKET,       /* ] */
    TOK_COMMA,          /* , */
    TOK_SEMICOLON,      /* ; */
} TokenType;

/* Token 结构 */
typedef struct {
    TokenType type;     /* Token 类型 */
    char *value;        /* Token 值 (标识符名/字符串/数字) */
    int line;           /* 行号 */
    int col;            /* 列号 */
} Token;

/* 获取 Token 类型名称 */
const char *token_type_name(TokenType type);

#endif /* TOKEN_H */
