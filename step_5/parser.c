/*
 * parser.c - 语法分析器实现
 * Step 3: 语法分析器 (递归下降)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "token.h"

/* 前向声明 */
static ASTNode *parse_statement(Parser *p);
static ASTNode *parse_expression(Parser *p);
static ASTNode *parse_block(Parser *p);

/* 报告错误 */
static void error(Parser *p, const char *msg) {
    fprintf(stderr, "错误 [%d:%d]: %s\n", p->current.line, p->current.col, msg);
    p->had_error = 1;
}

/* 前进到下一个 token */
static void advance(Parser *p) {
    p->previous = p->current;
    p->current = lexer_next(p->lexer);
}

/* 检查当前 token 类型 */
static int check(Parser *p, TokenType type) {
    return p->current.type == type;
}

/* 匹配并消费 token */
static int match(Parser *p, TokenType type) {
    if (check(p, type)) {
        advance(p);
        return 1;
    }
    return 0;
}

/* 期望指定 token，否则报错 */
static void expect(Parser *p, TokenType type, const char *msg) {
    if (!match(p, type)) {
        error(p, msg);
    }
}

/* 初始化语法分析器 */
void parser_init(Parser *p, Lexer *lexer) {
    p->lexer = lexer;
    p->had_error = 0;
    p->current.type = TOK_EOF;
    p->current.value = NULL;
    p->previous.type = TOK_EOF;
    p->previous.value = NULL;
    advance(p); /* 读取第一个 token */
}

/* 获取错误状态 */
int parser_had_error(Parser *p) {
    return p->had_error;
}

/* 解析基本表达式: 数字, 字符串, 标识符, 括号表达式 */
static ASTNode *parse_primary(Parser *p) {
    if (match(p, TOK_NUM)) {
        int value = atoi(p->previous.value);
        return ast_new_num(value, p->previous.line, p->previous.col);
    }

    if (match(p, TOK_STRING)) {
        return ast_new_string(p->previous.value, p->previous.line, p->previous.col);
    }

    if (match(p, TOK_IDENT)) {
        char *name = strdup(p->previous.value);
        int line = p->previous.line;
        int col = p->previous.col;

        /* 函数调用 */
        if (match(p, TOK_LPAREN)) {
            ASTList *args = ast_list_new();
            if (!check(p, TOK_RPAREN)) {
                do {
                    ASTNode *arg = parse_expression(p);
                    ast_list_add(args, arg);
                } while (match(p, TOK_COMMA));
            }
            expect(p, TOK_RPAREN, "期望 ')'");
            ASTNode *node = ast_new_call(name, args, line, col);
            free(name);
            return node;
        }

        ASTNode *node = ast_new_ident(name, line, col);
        free(name);
        return node;
    }

    if (match(p, TOK_LPAREN)) {
        ASTNode *expr = parse_expression(p);
        expect(p, TOK_RPAREN, "期望 ')'");
        return expr;
    }

    error(p, "期望表达式");
    return NULL;
}

/* 解析一元表达式: -x, !x */
static ASTNode *parse_unary(Parser *p) {
    if (match(p, TOK_MINUS) || match(p, TOK_NOT)) {
        int op = p->previous.type;
        int line = p->previous.line;
        int col = p->previous.col;
        ASTNode *operand = parse_unary(p);
        return ast_new_unary(op, operand, line, col);
    }
    return parse_primary(p);
}

/* 解析乘除: *, /, % */
static ASTNode *parse_factor(Parser *p) {
    ASTNode *left = parse_unary(p);

    while (match(p, TOK_STAR) || match(p, TOK_SLASH) || match(p, TOK_PERCENT)) {
        int op = p->previous.type;
        int line = p->previous.line;
        int col = p->previous.col;
        ASTNode *right = parse_unary(p);
        left = ast_new_binary(op, left, right, line, col);
    }

    return left;
}

/* 解析加减: +, - */
static ASTNode *parse_term(Parser *p) {
    ASTNode *left = parse_factor(p);

    while (match(p, TOK_PLUS) || match(p, TOK_MINUS)) {
        int op = p->previous.type;
        int line = p->previous.line;
        int col = p->previous.col;
        ASTNode *right = parse_factor(p);
        left = ast_new_binary(op, left, right, line, col);
    }

    return left;
}

/* 解析比较: <, <=, >, >= */
static ASTNode *parse_comparison(Parser *p) {
    ASTNode *left = parse_term(p);

    while (match(p, TOK_LT) || match(p, TOK_LE) ||
           match(p, TOK_GT) || match(p, TOK_GE)) {
        int op = p->previous.type;
        int line = p->previous.line;
        int col = p->previous.col;
        ASTNode *right = parse_term(p);
        left = ast_new_binary(op, left, right, line, col);
    }

    return left;
}

/* 解析相等: ==, != */
static ASTNode *parse_equality(Parser *p) {
    ASTNode *left = parse_comparison(p);

    while (match(p, TOK_EQ) || match(p, TOK_NE)) {
        int op = p->previous.type;
        int line = p->previous.line;
        int col = p->previous.col;
        ASTNode *right = parse_comparison(p);
        left = ast_new_binary(op, left, right, line, col);
    }

    return left;
}

/* 解析逻辑与: && */
static ASTNode *parse_and(Parser *p) {
    ASTNode *left = parse_equality(p);

    while (match(p, TOK_AND)) {
        int op = p->previous.type;
        int line = p->previous.line;
        int col = p->previous.col;
        ASTNode *right = parse_equality(p);
        left = ast_new_binary(op, left, right, line, col);
    }

    return left;
}

/* 解析逻辑或: || */
static ASTNode *parse_or(Parser *p) {
    ASTNode *left = parse_and(p);

    while (match(p, TOK_OR)) {
        int op = p->previous.type;
        int line = p->previous.line;
        int col = p->previous.col;
        ASTNode *right = parse_and(p);
        left = ast_new_binary(op, left, right, line, col);
    }

    return left;
}

/* 解析赋值表达式 */
static ASTNode *parse_assignment(Parser *p) {
    ASTNode *left = parse_or(p);

    if (match(p, TOK_ASSIGN)) {
        int line = p->previous.line;
        int col = p->previous.col;
        ASTNode *value = parse_assignment(p); /* 右结合 */

        if (left->type == AST_IDENT) {
            char *name = strdup(left->data.str_value);
            ast_free(left);
            return ast_new_assign(name, value, line, col);
        }

        error(p, "无效的赋值目标");
        ast_free(value);
    }

    return left;
}

/* 解析表达式 */
static ASTNode *parse_expression(Parser *p) {
    return parse_assignment(p);
}

/* 解析变量声明 */
static ASTNode *parse_var_decl(Parser *p, const char *type_name) {
    int line = p->current.line;
    int col = p->current.col;

    expect(p, TOK_IDENT, "期望变量名");
    char *var_name = strdup(p->previous.value);

    ASTNode *init = NULL;
    if (match(p, TOK_ASSIGN)) {
        init = parse_expression(p);
    }

    expect(p, TOK_SEMICOLON, "期望 ';'");

    ASTNode *node = ast_new_var_decl(type_name, var_name, init, line, col);
    free(var_name);
    return node;
}

/* 解析 return 语句 */
static ASTNode *parse_return(Parser *p) {
    int line = p->previous.line;
    int col = p->previous.col;

    ASTNode *value = NULL;
    if (!check(p, TOK_SEMICOLON)) {
        value = parse_expression(p);
    }

    expect(p, TOK_SEMICOLON, "期望 ';'");
    return ast_new_return(value, line, col);
}

/* 解析 if 语句 */
static ASTNode *parse_if(Parser *p) {
    int line = p->previous.line;
    int col = p->previous.col;

    expect(p, TOK_LPAREN, "期望 '('");
    ASTNode *cond = parse_expression(p);
    expect(p, TOK_RPAREN, "期望 ')'");

    ASTNode *then_branch = parse_statement(p);
    ASTNode *else_branch = NULL;

    if (match(p, TOK_ELSE)) {
        else_branch = parse_statement(p);
    }

    return ast_new_if(cond, then_branch, else_branch, line, col);
}

/* 解析 while 语句 */
static ASTNode *parse_while(Parser *p) {
    int line = p->previous.line;
    int col = p->previous.col;

    expect(p, TOK_LPAREN, "期望 '('");
    ASTNode *cond = parse_expression(p);
    expect(p, TOK_RPAREN, "期望 ')'");

    ASTNode *body = parse_statement(p);

    return ast_new_while(cond, body, line, col);
}

/* 解析代码块 */
static ASTNode *parse_block(Parser *p) {
    int line = p->previous.line;
    int col = p->previous.col;

    ASTNode *block = ast_new_block(line, col);

    while (!check(p, TOK_RBRACE) && !check(p, TOK_EOF)) {
        ASTNode *stmt = parse_statement(p);
        if (stmt) {
            ast_list_add(block->data.block, stmt);
        }
    }

    expect(p, TOK_RBRACE, "期望 '}'");
    return block;
}

/* 解析语句 */
static ASTNode *parse_statement(Parser *p) {
    /* 变量声明: int x; 或 int x = 10; */
    if (match(p, TOK_INT) || match(p, TOK_CHAR) || match(p, TOK_VOID)) {
        const char *type_name = token_type_name(p->previous.type);
        return parse_var_decl(p, type_name);
    }

    /* return 语句 */
    if (match(p, TOK_RETURN)) {
        return parse_return(p);
    }

    /* if 语句 */
    if (match(p, TOK_IF)) {
        return parse_if(p);
    }

    /* while 语句 */
    if (match(p, TOK_WHILE)) {
        return parse_while(p);
    }

    /* 代码块 */
    if (match(p, TOK_LBRACE)) {
        return parse_block(p);
    }

    /* 表达式语句 */
    int line = p->current.line;
    int col = p->current.col;
    ASTNode *expr = parse_expression(p);
    expect(p, TOK_SEMICOLON, "期望 ';'");
    return ast_new_expr_stmt(expr, line, col);
}

/* 解析函数定义 */
static ASTNode *parse_function(Parser *p) {
    /* 返回类型 */
    const char *ret_type = "int";
    if (match(p, TOK_INT) || match(p, TOK_CHAR) || match(p, TOK_VOID)) {
        ret_type = token_type_name(p->previous.type);
    } else {
        error(p, "期望返回类型");
        return NULL;
    }

    /* 函数名 */
    expect(p, TOK_IDENT, "期望函数名");
    char *name = strdup(p->previous.value);
    int line = p->previous.line;
    int col = p->previous.col;

    /* 参数列表 */
    expect(p, TOK_LPAREN, "期望 '('");
    ASTList *params = ast_list_new();

    if (!check(p, TOK_RPAREN)) {
        do {
            /* 解析参数: type name */
            const char *param_type = "int";
            if (match(p, TOK_INT) || match(p, TOK_CHAR)) {
                param_type = token_type_name(p->previous.type);
            } else {
                error(p, "期望参数类型");
                break;
            }

            expect(p, TOK_IDENT, "期望参数名");
            char *param_name = strdup(p->previous.value);

            ASTNode *param = ast_new_var_decl(param_type, param_name, NULL,
                                              p->previous.line, p->previous.col);
            free(param_name);
            ast_list_add(params, param);
        } while (match(p, TOK_COMMA));
    }
    expect(p, TOK_RPAREN, "期望 ')'");

    /* 函数体 */
    expect(p, TOK_LBRACE, "期望 '{'");
    ASTNode *body = parse_block(p);

    ASTNode *func = ast_new_func_def(ret_type, name, params, body, line, col);
    free(name);
    return func;
}

/* 解析程序 */
ASTNode *parse_program(Parser *p) {
    ASTNode *program = ast_new_program(1, 1);

    while (!check(p, TOK_EOF)) {
        ASTNode *func = parse_function(p);
        if (func) {
            ast_list_add(program->data.block, func);
        }
        if (p->had_error) break;
    }

    return program;
}
