/*
 * semantic.c - 语义分析器实现
 * Step 4: 语义分析
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"
#include "token.h"

/* 报告语义错误 */
static void semantic_error(SemanticAnalyzer *a, int line, int col, const char *msg) {
    fprintf(stderr, "语义错误 [%d:%d]: %s\n", line, col, msg);
    a->error_count++;
}

/* 前向声明 */
static DataType analyze_expr(SemanticAnalyzer *a, ASTNode *node);
static void analyze_stmt(SemanticAnalyzer *a, ASTNode *node);

/* 分析表达式，返回表达式类型 */
static DataType analyze_expr(SemanticAnalyzer *a, ASTNode *node) {
    if (!node) return TYPE_VOID;

    switch (node->type) {
        case AST_NUM:
            return TYPE_INT;

        case AST_STRING:
            return TYPE_CHAR; /* 简化处理 */

        case AST_IDENT: {
            Symbol *sym = symtab_lookup(a->current_scope, node->data.str_value);
            if (!sym) {
                char msg[256];
                snprintf(msg, sizeof(msg), "未定义的变量 '%s'", node->data.str_value);
                semantic_error(a, node->line, node->col, msg);
                return TYPE_UNKNOWN;
            }
            return sym->type;
        }

        case AST_BINARY: {
            DataType left = analyze_expr(a, node->data.binary.left);
            DataType right = analyze_expr(a, node->data.binary.right);

            /* 简单类型检查: 两边类型应该兼容 */
            if (left != right && left != TYPE_UNKNOWN && right != TYPE_UNKNOWN) {
                semantic_error(a, node->line, node->col, "二元运算类型不匹配");
            }

            /* 比较运算返回 int (作为 bool) */
            int op = node->data.binary.op;
            if (op == TOK_EQ || op == TOK_NE || op == TOK_LT ||
                op == TOK_LE || op == TOK_GT || op == TOK_GE ||
                op == TOK_AND || op == TOK_OR) {
                return TYPE_INT;
            }

            return left != TYPE_UNKNOWN ? left : right;
        }

        case AST_UNARY: {
            DataType operand = analyze_expr(a, node->data.unary.operand);
            return operand;
        }

        case AST_ASSIGN: {
            Symbol *sym = symtab_lookup(a->current_scope, node->data.assign.name);
            if (!sym) {
                char msg[256];
                snprintf(msg, sizeof(msg), "未定义的变量 '%s'", node->data.assign.name);
                semantic_error(a, node->line, node->col, msg);
                return TYPE_UNKNOWN;
            }

            DataType value_type = analyze_expr(a, node->data.assign.value);
            if (sym->type != value_type && value_type != TYPE_UNKNOWN) {
                semantic_error(a, node->line, node->col, "赋值类型不匹配");
            }
            return sym->type;
        }

        case AST_CALL: {
            /* 简化: 假设函数调用返回 int */
            Symbol *sym = symtab_lookup(a->current_scope, node->data.call.func_name);
            if (!sym) {
                /* 允许未声明的函数 (简化处理) */
            }
            /* 分析参数 */
            if (node->data.call.args) {
                for (int i = 0; i < node->data.call.args->count; i++) {
                    analyze_expr(a, node->data.call.args->nodes[i]);
                }
            }
            return TYPE_INT;
        }

        default:
            return TYPE_UNKNOWN;
    }
}

/* 分析语句 */
static void analyze_stmt(SemanticAnalyzer *a, ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case AST_VAR_DECL: {
            /* 检查重复定义 */
            if (symtab_lookup_local(a->current_scope, node->data.var_decl.var_name)) {
                char msg[256];
                snprintf(msg, sizeof(msg), "变量 '%s' 重复定义",
                         node->data.var_decl.var_name);
                semantic_error(a, node->line, node->col, msg);
                return;
            }

            /* 添加到符号表 */
            DataType type = type_from_name(node->data.var_decl.var_type);
            symtab_add(a->current_scope, node->data.var_decl.var_name, SYM_VAR, type);

            /* 分析初始值 */
            if (node->data.var_decl.init) {
                DataType init_type = analyze_expr(a, node->data.var_decl.init);
                if (type != init_type && init_type != TYPE_UNKNOWN) {
                    semantic_error(a, node->line, node->col, "初始化类型不匹配");
                }
            }
            break;
        }

        case AST_RETURN: {
            if (node->data.ret.value) {
                analyze_expr(a, node->data.ret.value);
            }
            break;
        }

        case AST_IF: {
            analyze_expr(a, node->data.if_stmt.cond);
            analyze_stmt(a, node->data.if_stmt.then_branch);
            if (node->data.if_stmt.else_branch) {
                analyze_stmt(a, node->data.if_stmt.else_branch);
            }
            break;
        }

        case AST_WHILE: {
            analyze_expr(a, node->data.while_stmt.cond);
            analyze_stmt(a, node->data.while_stmt.body);
            break;
        }

        case AST_BLOCK: {
            /* 进入新作用域 */
            a->current_scope = symtab_enter_scope(a->current_scope);

            for (int i = 0; i < node->data.block->count; i++) {
                analyze_stmt(a, node->data.block->nodes[i]);
            }

            /* 离开作用域 */
            a->current_scope = symtab_leave_scope(a->current_scope);
            break;
        }

        case AST_EXPR_STMT: {
            analyze_expr(a, node->data.ret.value);
            break;
        }

        default:
            break;
    }
}

/* 分析函数定义 */
static void analyze_func(SemanticAnalyzer *a, ASTNode *node) {
    if (!node || node->type != AST_FUNC_DEF) return;

    /* 添加函数到符号表 */
    DataType ret_type = type_from_name(node->data.func_def.return_type);
    symtab_add(a->current_scope, node->data.func_def.name, SYM_FUNC, ret_type);

    /* 进入函数作用域 */
    a->current_scope = symtab_enter_scope(a->current_scope);

    /* 添加参数到符号表 */
    ASTList *params = node->data.func_def.params;
    if (params) {
        for (int i = 0; i < params->count; i++) {
            ASTNode *param = params->nodes[i];
            DataType param_type = type_from_name(param->data.var_decl.var_type);
            symtab_add(a->current_scope, param->data.var_decl.var_name, SYM_VAR, param_type);
        }
    }

    /* 分析函数体 */
    if (node->data.func_def.body) {
        /* 直接分析 block 内容，不再创建额外作用域 */
        ASTNode *body = node->data.func_def.body;
        if (body->type == AST_BLOCK) {
            for (int i = 0; i < body->data.block->count; i++) {
                analyze_stmt(a, body->data.block->nodes[i]);
            }
        }
    }

    /* 离开函数作用域 */
    a->current_scope = symtab_leave_scope(a->current_scope);
}

/* 初始化语义分析器 */
void semantic_init(SemanticAnalyzer *a) {
    a->current_scope = symtab_new(NULL);
    a->error_count = 0;
}

/* 释放语义分析器 */
void semantic_free(SemanticAnalyzer *a) {
    while (a->current_scope) {
        a->current_scope = symtab_leave_scope(a->current_scope);
    }
}

/* 分析 AST */
int semantic_analyze(SemanticAnalyzer *a, ASTNode *ast) {
    if (!ast) return -1;

    if (ast->type == AST_PROGRAM) {
        for (int i = 0; i < ast->data.block->count; i++) {
            analyze_func(a, ast->data.block->nodes[i]);
        }
    }

    return a->error_count;
}

/* 获取错误数 */
int semantic_error_count(SemanticAnalyzer *a) {
    return a->error_count;
}
