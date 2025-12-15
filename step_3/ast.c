/*
 * ast.c - AST 实现
 * Step 3: 语法分析器
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "token.h"

/* 创建基础节点 */
static ASTNode *ast_new(ASTNodeType type, int line, int col) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    node->type = type;
    node->line = line;
    node->col = col;
    return node;
}

/* 创建数字节点 */
ASTNode *ast_new_num(int value, int line, int col) {
    ASTNode *node = ast_new(AST_NUM, line, col);
    node->data.num_value = value;
    return node;
}

/* 创建字符串节点 */
ASTNode *ast_new_string(const char *value, int line, int col) {
    ASTNode *node = ast_new(AST_STRING, line, col);
    node->data.str_value = strdup(value);
    return node;
}

/* 创建标识符节点 */
ASTNode *ast_new_ident(const char *name, int line, int col) {
    ASTNode *node = ast_new(AST_IDENT, line, col);
    node->data.str_value = strdup(name);
    return node;
}

/* 创建二元运算节点 */
ASTNode *ast_new_binary(int op, ASTNode *left, ASTNode *right, int line, int col) {
    ASTNode *node = ast_new(AST_BINARY, line, col);
    node->data.binary.op = op;
    node->data.binary.left = left;
    node->data.binary.right = right;
    return node;
}

/* 创建一元运算节点 */
ASTNode *ast_new_unary(int op, ASTNode *operand, int line, int col) {
    ASTNode *node = ast_new(AST_UNARY, line, col);
    node->data.unary.op = op;
    node->data.unary.operand = operand;
    return node;
}

/* 创建赋值节点 */
ASTNode *ast_new_assign(const char *name, ASTNode *value, int line, int col) {
    ASTNode *node = ast_new(AST_ASSIGN, line, col);
    node->data.assign.name = strdup(name);
    node->data.assign.value = value;
    return node;
}

/* 创建变量声明节点 */
ASTNode *ast_new_var_decl(const char *type, const char *name, ASTNode *init, int line, int col) {
    ASTNode *node = ast_new(AST_VAR_DECL, line, col);
    node->data.var_decl.var_type = strdup(type);
    node->data.var_decl.var_name = strdup(name);
    node->data.var_decl.init = init;
    return node;
}

/* 创建 return 节点 */
ASTNode *ast_new_return(ASTNode *value, int line, int col) {
    ASTNode *node = ast_new(AST_RETURN, line, col);
    node->data.ret.value = value;
    return node;
}

/* 创建 if 节点 */
ASTNode *ast_new_if(ASTNode *cond, ASTNode *then_b, ASTNode *else_b, int line, int col) {
    ASTNode *node = ast_new(AST_IF, line, col);
    node->data.if_stmt.cond = cond;
    node->data.if_stmt.then_branch = then_b;
    node->data.if_stmt.else_branch = else_b;
    return node;
}

/* 创建 while 节点 */
ASTNode *ast_new_while(ASTNode *cond, ASTNode *body, int line, int col) {
    ASTNode *node = ast_new(AST_WHILE, line, col);
    node->data.while_stmt.cond = cond;
    node->data.while_stmt.body = body;
    return node;
}

/* 创建代码块节点 */
ASTNode *ast_new_block(int line, int col) {
    ASTNode *node = ast_new(AST_BLOCK, line, col);
    node->data.block = ast_list_new();
    return node;
}

/* 创建函数定义节点 */
ASTNode *ast_new_func_def(const char *ret_type, const char *name, ASTNode *body, int line, int col) {
    ASTNode *node = ast_new(AST_FUNC_DEF, line, col);
    node->data.func_def.return_type = strdup(ret_type);
    node->data.func_def.name = strdup(name);
    node->data.func_def.body = body;
    return node;
}

/* 创建程序节点 */
ASTNode *ast_new_program(int line, int col) {
    ASTNode *node = ast_new(AST_PROGRAM, line, col);
    node->data.block = ast_list_new();
    return node;
}

/* 创建函数调用节点 */
ASTNode *ast_new_call(const char *name, ASTList *args, int line, int col) {
    ASTNode *node = ast_new(AST_CALL, line, col);
    node->data.call.func_name = strdup(name);
    node->data.call.args = args;
    return node;
}

/* 创建表达式语句节点 */
ASTNode *ast_new_expr_stmt(ASTNode *expr, int line, int col) {
    ASTNode *node = ast_new(AST_EXPR_STMT, line, col);
    node->data.ret.value = expr; /* 复用 ret 结构 */
    return node;
}

/* 创建 AST 列表 */
ASTList *ast_list_new(void) {
    ASTList *list = malloc(sizeof(ASTList));
    list->nodes = NULL;
    list->count = 0;
    list->capacity = 0;
    return list;
}

/* 添加节点到列表 */
void ast_list_add(ASTList *list, ASTNode *node) {
    if (list->count >= list->capacity) {
        list->capacity = list->capacity == 0 ? 4 : list->capacity * 2;
        list->nodes = realloc(list->nodes, list->capacity * sizeof(ASTNode *));
    }
    list->nodes[list->count++] = node;
}

/* 释放 AST 列表 */
static void ast_list_free(ASTList *list) {
    if (!list) return;
    for (int i = 0; i < list->count; i++) {
        ast_free(list->nodes[i]);
    }
    free(list->nodes);
    free(list);
}

/* 释放 AST 节点 */
void ast_free(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case AST_STRING:
        case AST_IDENT:
            free(node->data.str_value);
            break;
        case AST_FUNC_DEF:
            free(node->data.func_def.name);
            free(node->data.func_def.return_type);
            ast_free(node->data.func_def.body);
            break;
        case AST_VAR_DECL:
            free(node->data.var_decl.var_type);
            free(node->data.var_decl.var_name);
            ast_free(node->data.var_decl.init);
            break;
        case AST_BINARY:
            ast_free(node->data.binary.left);
            ast_free(node->data.binary.right);
            break;
        case AST_UNARY:
            ast_free(node->data.unary.operand);
            break;
        case AST_ASSIGN:
            free(node->data.assign.name);
            ast_free(node->data.assign.value);
            break;
        case AST_RETURN:
        case AST_EXPR_STMT:
            ast_free(node->data.ret.value);
            break;
        case AST_IF:
            ast_free(node->data.if_stmt.cond);
            ast_free(node->data.if_stmt.then_branch);
            ast_free(node->data.if_stmt.else_branch);
            break;
        case AST_WHILE:
            ast_free(node->data.while_stmt.cond);
            ast_free(node->data.while_stmt.body);
            break;
        case AST_CALL:
            free(node->data.call.func_name);
            ast_list_free(node->data.call.args);
            break;
        case AST_BLOCK:
        case AST_PROGRAM:
            ast_list_free(node->data.block);
            break;
        default:
            break;
    }
    free(node);
}

/* 打印缩进 */
static void print_indent(int indent) {
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
}

/* 获取运算符名称 */
static const char *op_name(int op) {
    switch (op) {
        case TOK_PLUS: return "+";
        case TOK_MINUS: return "-";
        case TOK_STAR: return "*";
        case TOK_SLASH: return "/";
        case TOK_PERCENT: return "%";
        case TOK_EQ: return "==";
        case TOK_NE: return "!=";
        case TOK_LT: return "<";
        case TOK_LE: return "<=";
        case TOK_GT: return ">";
        case TOK_GE: return ">=";
        case TOK_AND: return "&&";
        case TOK_OR: return "||";
        case TOK_NOT: return "!";
        default: return "?";
    }
}

/* 打印 AST */
void ast_print(ASTNode *node, int indent) {
    if (!node) return;

    print_indent(indent);

    switch (node->type) {
        case AST_PROGRAM:
            printf("Program\n");
            for (int i = 0; i < node->data.block->count; i++) {
                ast_print(node->data.block->nodes[i], indent + 1);
            }
            break;

        case AST_FUNC_DEF:
            printf("FuncDef: %s %s()\n",
                   node->data.func_def.return_type,
                   node->data.func_def.name);
            ast_print(node->data.func_def.body, indent + 1);
            break;

        case AST_BLOCK:
            printf("Block\n");
            for (int i = 0; i < node->data.block->count; i++) {
                ast_print(node->data.block->nodes[i], indent + 1);
            }
            break;

        case AST_VAR_DECL:
            printf("VarDecl: %s %s",
                   node->data.var_decl.var_type,
                   node->data.var_decl.var_name);
            if (node->data.var_decl.init) {
                printf(" =\n");
                ast_print(node->data.var_decl.init, indent + 1);
            } else {
                printf("\n");
            }
            break;

        case AST_RETURN:
            printf("Return\n");
            if (node->data.ret.value) {
                ast_print(node->data.ret.value, indent + 1);
            }
            break;

        case AST_IF:
            printf("If\n");
            print_indent(indent + 1);
            printf("Cond:\n");
            ast_print(node->data.if_stmt.cond, indent + 2);
            print_indent(indent + 1);
            printf("Then:\n");
            ast_print(node->data.if_stmt.then_branch, indent + 2);
            if (node->data.if_stmt.else_branch) {
                print_indent(indent + 1);
                printf("Else:\n");
                ast_print(node->data.if_stmt.else_branch, indent + 2);
            }
            break;

        case AST_WHILE:
            printf("While\n");
            print_indent(indent + 1);
            printf("Cond:\n");
            ast_print(node->data.while_stmt.cond, indent + 2);
            print_indent(indent + 1);
            printf("Body:\n");
            ast_print(node->data.while_stmt.body, indent + 2);
            break;

        case AST_EXPR_STMT:
            printf("ExprStmt\n");
            ast_print(node->data.ret.value, indent + 1);
            break;

        case AST_NUM:
            printf("Num: %d\n", node->data.num_value);
            break;

        case AST_STRING:
            printf("String: \"%s\"\n", node->data.str_value);
            break;

        case AST_IDENT:
            printf("Ident: %s\n", node->data.str_value);
            break;

        case AST_BINARY:
            printf("Binary: %s\n", op_name(node->data.binary.op));
            ast_print(node->data.binary.left, indent + 1);
            ast_print(node->data.binary.right, indent + 1);
            break;

        case AST_UNARY:
            printf("Unary: %s\n", op_name(node->data.unary.op));
            ast_print(node->data.unary.operand, indent + 1);
            break;

        case AST_ASSIGN:
            printf("Assign: %s =\n", node->data.assign.name);
            ast_print(node->data.assign.value, indent + 1);
            break;

        case AST_CALL:
            printf("Call: %s()\n", node->data.call.func_name);
            if (node->data.call.args) {
                for (int i = 0; i < node->data.call.args->count; i++) {
                    ast_print(node->data.call.args->nodes[i], indent + 1);
                }
            }
            break;
    }
}
