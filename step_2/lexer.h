/*
 * lexer.h - 词法分析器头文件
 * Step 2: 词法分析器
 */

#ifndef LEXER_H
#define LEXER_H

#include "token.h"

/* 词法分析器状态 */
typedef struct {
    const char *source; /* 源代码 */
    const char *pos;    /* 当前位置 */
    int line;           /* 当前行号 */
    int col;            /* 当前列号 */
    Token current;      /* 当前 token */
} Lexer;

/* 初始化词法分析器 */
void lexer_init(Lexer *lexer, const char *source);

/* 获取下一个 token */
Token lexer_next(Lexer *lexer);

/* 查看当前 token (不消费) */
Token lexer_peek(Lexer *lexer);

/* 释放 token 资源 */
void token_free(Token *token);

#endif /* LEXER_H */
