/*
 * ast.h - 抽象语法树定义
 * Step 3: 语法分析器
 */

#ifndef AST_H
#define AST_H

/* AST 节点类型 */
typedef enum {
    /* 程序结构 */
    AST_PROGRAM,        /* 程序 (函数列表) */
    AST_FUNC_DEF,       /* 函数定义 */
    AST_BLOCK,          /* 代码块 {} */

    /* 语句 */
    AST_VAR_DECL,       /* 变量声明 */
    AST_RETURN,         /* return 语句 */
    AST_IF,             /* if 语句 */
    AST_WHILE,          /* while 语句 */
    AST_EXPR_STMT,      /* 表达式语句 */

    /* 表达式 */
    AST_NUM,            /* 数字字面量 */
    AST_STRING,         /* 字符串字面量 */
    AST_IDENT,          /* 标识符 */
    AST_BINARY,         /* 二元运算 */
    AST_UNARY,          /* 一元运算 */
    AST_ASSIGN,         /* 赋值 */
    AST_CALL,           /* 函数调用 */
} ASTNodeType;

/* 前向声明 */
typedef struct ASTNode ASTNode;

/* AST 节点列表 */
typedef struct {
    ASTNode **nodes;
    int count;
    int capacity;
} ASTList;

/* AST 节点 */
struct ASTNode {
    ASTNodeType type;
    int line;
    int col;

    union {
        /* AST_NUM */
        int num_value;

        /* AST_STRING, AST_IDENT */
        char *str_value;

        /* AST_FUNC_DEF: 函数定义 */
        struct {
            char *name;
            char *return_type;
            ASTNode *body;      /* AST_BLOCK */
        } func_def;

        /* AST_VAR_DECL: 变量声明 */
        struct {
            char *var_type;
            char *var_name;
            ASTNode *init;      /* 初始值 (可为 NULL) */
        } var_decl;

        /* AST_BINARY: 二元运算 */
        struct {
            int op;             /* 运算符 (TokenType) */
            ASTNode *left;
            ASTNode *right;
        } binary;

        /* AST_UNARY: 一元运算 */
        struct {
            int op;
            ASTNode *operand;
        } unary;

        /* AST_ASSIGN: 赋值 */
        struct {
            char *name;
            ASTNode *value;
        } assign;

        /* AST_RETURN */
        struct {
            ASTNode *value;     /* 可为 NULL */
        } ret;

        /* AST_IF */
        struct {
            ASTNode *cond;
            ASTNode *then_branch;
            ASTNode *else_branch; /* 可为 NULL */
        } if_stmt;

        /* AST_WHILE */
        struct {
            ASTNode *cond;
            ASTNode *body;
        } while_stmt;

        /* AST_CALL: 函数调用 */
        struct {
            char *func_name;
            ASTList *args;
        } call;

        /* AST_BLOCK, AST_PROGRAM */
        ASTList *block;
    } data;
};

/* 创建 AST 节点 */
ASTNode *ast_new_num(int value, int line, int col);
ASTNode *ast_new_string(const char *value, int line, int col);
ASTNode *ast_new_ident(const char *name, int line, int col);
ASTNode *ast_new_binary(int op, ASTNode *left, ASTNode *right, int line, int col);
ASTNode *ast_new_unary(int op, ASTNode *operand, int line, int col);
ASTNode *ast_new_assign(const char *name, ASTNode *value, int line, int col);
ASTNode *ast_new_var_decl(const char *type, const char *name, ASTNode *init, int line, int col);
ASTNode *ast_new_return(ASTNode *value, int line, int col);
ASTNode *ast_new_if(ASTNode *cond, ASTNode *then_b, ASTNode *else_b, int line, int col);
ASTNode *ast_new_while(ASTNode *cond, ASTNode *body, int line, int col);
ASTNode *ast_new_block(int line, int col);
ASTNode *ast_new_func_def(const char *ret_type, const char *name, ASTNode *body, int line, int col);
ASTNode *ast_new_program(int line, int col);
ASTNode *ast_new_call(const char *name, ASTList *args, int line, int col);
ASTNode *ast_new_expr_stmt(ASTNode *expr, int line, int col);

/* AST 列表操作 */
ASTList *ast_list_new(void);
void ast_list_add(ASTList *list, ASTNode *node);

/* 释放 AST */
void ast_free(ASTNode *node);

/* 打印 AST */
void ast_print(ASTNode *node, int indent);

#endif /* AST_H */
