/*
 * parser.h - 语法分析器头文件
 * Step 3: 语法分析器
 */

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

/* 语法分析器状态 */
typedef struct {
    Lexer *lexer;
    Token current;
    Token previous;
    int had_error;
} Parser;

/* 初始化语法分析器 */
void parser_init(Parser *parser, Lexer *lexer);

/* 解析程序 */
ASTNode *parse_program(Parser *parser);

/* 获取错误状态 */
int parser_had_error(Parser *parser);

#endif /* PARSER_H */
