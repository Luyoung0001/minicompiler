/*
 * codegen.c - RISC-V 32 代码生成器实现
 * Step 5: 代码生成
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"
#include "token.h"

/* 输出汇编指令 */
#define EMIT(gen, ...) fprintf((gen)->out, __VA_ARGS__)

/* 生成唯一标签 */
static int new_label(CodeGen *gen) {
    return gen->label_count++;
}

/* 前向声明 */
static void gen_expr(CodeGen *gen, ASTNode *node);
static void gen_stmt(CodeGen *gen, ASTNode *node);

/* 查找变量的栈偏移 */
static int get_var_offset(CodeGen *gen, const char *name) {
    Symbol *sym = symtab_lookup(gen->symbols, name);
    if (sym) {
        return sym->offset;
    }
    return 0;
}

/* 生成表达式代码，结果存入 a0 */
static void gen_expr(CodeGen *gen, ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case AST_NUM:
            EMIT(gen, "    li a0, %d\n", node->data.num_value);
            break;

        case AST_IDENT: {
            int offset = get_var_offset(gen, node->data.str_value);
            EMIT(gen, "    lw a0, -%d(s0)\n", offset);
            break;
        }

        case AST_BINARY: {
            /* 先计算右操作数，压栈 */
            gen_expr(gen, node->data.binary.right);
            EMIT(gen, "    addi sp, sp, -4\n");
            EMIT(gen, "    sw a0, 0(sp)\n");
            gen->stack_size += 4;

            /* 再计算左操作数 */
            gen_expr(gen, node->data.binary.left);

            /* 从栈中弹出右操作数到 a1 */
            EMIT(gen, "    lw a1, 0(sp)\n");
            EMIT(gen, "    addi sp, sp, 4\n");
            gen->stack_size -= 4;

            /* 执行运算 */
            switch (node->data.binary.op) {
                case TOK_PLUS:
                    EMIT(gen, "    add a0, a0, a1\n");
                    break;
                case TOK_MINUS:
                    EMIT(gen, "    sub a0, a0, a1\n");
                    break;
                case TOK_STAR:
                    EMIT(gen, "    mul a0, a0, a1\n");
                    break;
                case TOK_SLASH:
                    EMIT(gen, "    div a0, a0, a1\n");
                    break;
                case TOK_PERCENT:
                    EMIT(gen, "    rem a0, a0, a1\n");
                    break;
                case TOK_EQ:
                    EMIT(gen, "    sub a0, a0, a1\n");
                    EMIT(gen, "    seqz a0, a0\n");
                    break;
                case TOK_NE:
                    EMIT(gen, "    sub a0, a0, a1\n");
                    EMIT(gen, "    snez a0, a0\n");
                    break;
                case TOK_LT:
                    EMIT(gen, "    slt a0, a0, a1\n");
                    break;
                case TOK_LE:
                    EMIT(gen, "    slt a0, a1, a0\n");
                    EMIT(gen, "    xori a0, a0, 1\n");
                    break;
                case TOK_GT:
                    EMIT(gen, "    slt a0, a1, a0\n");
                    break;
                case TOK_GE:
                    EMIT(gen, "    slt a0, a0, a1\n");
                    EMIT(gen, "    xori a0, a0, 1\n");
                    break;
                case TOK_AND:
                    EMIT(gen, "    snez a0, a0\n");
                    EMIT(gen, "    snez a1, a1\n");
                    EMIT(gen, "    and a0, a0, a1\n");
                    break;
                case TOK_OR:
                    EMIT(gen, "    or a0, a0, a1\n");
                    EMIT(gen, "    snez a0, a0\n");
                    break;
                default:
                    break;
            }
            break;
        }

        case AST_UNARY: {
            gen_expr(gen, node->data.unary.operand);
            switch (node->data.unary.op) {
                case TOK_MINUS:
                    EMIT(gen, "    neg a0, a0\n");
                    break;
                case TOK_NOT:
                    EMIT(gen, "    seqz a0, a0\n");
                    break;
                default:
                    break;
            }
            break;
        }

        case AST_ASSIGN: {
            gen_expr(gen, node->data.assign.value);
            int offset = get_var_offset(gen, node->data.assign.name);
            EMIT(gen, "    sw a0, -%d(s0)\n", offset);
            break;
        }

        case AST_CALL: {
            /* 函数调用: 参数通过 a0-a7 传递 */
            ASTList *args = node->data.call.args;
            int num_args = args ? args->count : 0;

            /* 先将所有参数压栈（从右到左计算） */
            for (int i = num_args - 1; i >= 0; i--) {
                gen_expr(gen, args->nodes[i]);
                EMIT(gen, "    addi sp, sp, -4\n");
                EMIT(gen, "    sw a0, 0(sp)\n");
            }

            /* 从栈中弹出参数到 a0-a7 */
            for (int i = 0; i < num_args && i < 8; i++) {
                EMIT(gen, "    lw a%d, %d(sp)\n", i, i * 4);
            }
            if (num_args > 0) {
                EMIT(gen, "    addi sp, sp, %d\n", num_args * 4);
            }

            /* 调用函数 */
            EMIT(gen, "    call %s\n", node->data.call.func_name);
            /* 返回值已在 a0 */
            break;
        }

        default:
            break;
    }
}

/* 生成语句代码 */
static void gen_stmt(CodeGen *gen, ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case AST_VAR_DECL: {
            /* 为变量分配栈空间 */
            Symbol *sym = symtab_lookup_local(gen->symbols, node->data.var_decl.var_name);
            if (!sym) {
                sym = symtab_add(gen->symbols, node->data.var_decl.var_name,
                                 SYM_VAR, type_from_name(node->data.var_decl.var_type));
            }

            /* 如果有初始值，生成代码 */
            if (node->data.var_decl.init) {
                gen_expr(gen, node->data.var_decl.init);
                EMIT(gen, "    sw a0, -%d(s0)\n", sym->offset);
            }
            break;
        }

        case AST_RETURN: {
            if (node->data.ret.value) {
                gen_expr(gen, node->data.ret.value);
            }
            /* 跳转到函数结尾 */
            EMIT(gen, "    j .Lret%d\n", gen->current_ret_label);
            break;
        }

        case AST_IF: {
            int else_label = new_label(gen);
            int end_label = new_label(gen);

            /* 计算条件 */
            gen_expr(gen, node->data.if_stmt.cond);
            EMIT(gen, "    beqz a0, .L%d\n", else_label);

            /* then 分支 */
            gen_stmt(gen, node->data.if_stmt.then_branch);
            if (node->data.if_stmt.else_branch) {
                EMIT(gen, "    j .L%d\n", end_label);
            }

            /* else 分支 */
            EMIT(gen, ".L%d:\n", else_label);
            if (node->data.if_stmt.else_branch) {
                gen_stmt(gen, node->data.if_stmt.else_branch);
                EMIT(gen, ".L%d:\n", end_label);
            }
            break;
        }

        case AST_WHILE: {
            int start_label = new_label(gen);
            int end_label = new_label(gen);

            EMIT(gen, ".L%d:\n", start_label);

            /* 计算条件 */
            gen_expr(gen, node->data.while_stmt.cond);
            EMIT(gen, "    beqz a0, .L%d\n", end_label);

            /* 循环体 */
            gen_stmt(gen, node->data.while_stmt.body);
            EMIT(gen, "    j .L%d\n", start_label);

            EMIT(gen, ".L%d:\n", end_label);
            break;
        }

        case AST_BLOCK: {
            /* 进入新作用域 */
            gen->symbols = symtab_enter_scope(gen->symbols);

            for (int i = 0; i < node->data.block->count; i++) {
                gen_stmt(gen, node->data.block->nodes[i]);
            }

            /* 离开作用域 */
            gen->symbols = symtab_leave_scope(gen->symbols);
            break;
        }

        case AST_EXPR_STMT: {
            gen_expr(gen, node->data.ret.value);
            break;
        }

        default:
            break;
    }
}

/* 生成函数代码 */
static void gen_function(CodeGen *gen, ASTNode *node) {
    if (!node || node->type != AST_FUNC_DEF) return;

    const char *name = node->data.func_def.name;
    ASTList *params = node->data.func_def.params;
    int num_params = params ? params->count : 0;
    int ret_label = new_label(gen);  /* 每个函数唯一的返回标签 */

    /* 进入函数作用域 */
    gen->symbols = symtab_enter_scope(gen->symbols);
    gen->symbols->stack_offset = 8;  /* 跳过保存的 ra 和 s0 */

    /* 计算需要的栈空间 (简化: 固定 64 字节) */
    int frame_size = 64;

    EMIT(gen, "\n");
    EMIT(gen, "    .globl %s\n", name);
    EMIT(gen, "%s:\n", name);

    /* 函数 prologue */
    EMIT(gen, "    addi sp, sp, -%d\n", frame_size);
    EMIT(gen, "    sw ra, %d(sp)\n", frame_size - 4);
    EMIT(gen, "    sw s0, %d(sp)\n", frame_size - 8);
    EMIT(gen, "    addi s0, sp, %d\n", frame_size);

    /* 保存参数到栈上，并添加到符号表 */
    for (int i = 0; i < num_params && i < 8; i++) {
        ASTNode *param = params->nodes[i];
        Symbol *sym = symtab_add(gen->symbols, param->data.var_decl.var_name,
                                  SYM_VAR, type_from_name(param->data.var_decl.var_type));
        EMIT(gen, "    sw a%d, -%d(s0)\n", i, sym->offset);
    }

    /* 保存当前函数的返回标签，供 return 语句使用 */
    int saved_ret_label = gen->current_ret_label;
    gen->current_ret_label = ret_label;

    /* 生成函数体 */
    if (node->data.func_def.body) {
        ASTNode *body = node->data.func_def.body;
        if (body->type == AST_BLOCK) {
            for (int i = 0; i < body->data.block->count; i++) {
                gen_stmt(gen, body->data.block->nodes[i]);
            }
        }
    }

    /* 恢复返回标签 */
    gen->current_ret_label = saved_ret_label;

    /* 函数 epilogue */
    EMIT(gen, ".Lret%d:\n", ret_label);
    EMIT(gen, "    lw ra, %d(sp)\n", frame_size - 4);
    EMIT(gen, "    lw s0, %d(sp)\n", frame_size - 8);
    EMIT(gen, "    addi sp, sp, %d\n", frame_size);
    EMIT(gen, "    ret\n");

    /* 离开函数作用域 */
    gen->symbols = symtab_leave_scope(gen->symbols);
}

/* 初始化代码生成器 */
void codegen_init(CodeGen *gen, FILE *out) {
    gen->out = out;
    gen->symbols = symtab_new(NULL);
    gen->label_count = 0;
    gen->stack_size = 0;
    gen->current_ret_label = 0;
}

/* 释放代码生成器 */
void codegen_free(CodeGen *gen) {
    while (gen->symbols) {
        gen->symbols = symtab_leave_scope(gen->symbols);
    }
}

/* 生成程序代码 */
int codegen_generate(CodeGen *gen, ASTNode *ast) {
    if (!ast) return -1;

    /* 文件头 */
    EMIT(gen, "# Generated by minicc\n");
    EMIT(gen, "# Target: RISC-V 32\n\n");
    EMIT(gen, "    .text\n");

    /* 生成 _start 入口点 */
    EMIT(gen, "    .globl _start\n");
    EMIT(gen, "_start:\n");
    EMIT(gen, "    call main\n");
    EMIT(gen, "    # exit(a0) - 返回值已在 a0 中\n");
    EMIT(gen, "    li a7, 93\n");
    EMIT(gen, "    ecall\n");

    /* 生成所有函数 */
    if (ast->type == AST_PROGRAM) {
        for (int i = 0; i < ast->data.block->count; i++) {
            gen_function(gen, ast->data.block->nodes[i]);
        }
    }

    return 0;
}
