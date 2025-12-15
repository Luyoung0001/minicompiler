/*
 * codegen.h - 代码生成器头文件
 * Step 5: 代码生成
 */

#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>
#include "ast.h"
#include "symbol.h"

/* 代码生成器状态 */
typedef struct {
    FILE *out;              /* 输出文件 */
    SymbolTable *symbols;   /* 符号表 */
    int label_count;        /* 标签计数 */
    int stack_size;         /* 当前栈大小 */
    int current_ret_label;  /* 当前函数的返回标签 */
} CodeGen;

/* 初始化代码生成器 */
void codegen_init(CodeGen *gen, FILE *out);

/* 释放代码生成器 */
void codegen_free(CodeGen *gen);

/* 生成程序代码 */
int codegen_generate(CodeGen *gen, ASTNode *ast);

#endif /* CODEGEN_H */
