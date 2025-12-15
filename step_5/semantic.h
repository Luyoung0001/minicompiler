/*
 * semantic.h - 语义分析器头文件
 * Step 4: 语义分析
 */

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "symbol.h"

/* 语义分析器状态 */
typedef struct {
    SymbolTable *current_scope;  /* 当前符号表 */
    int error_count;             /* 错误计数 */
} SemanticAnalyzer;

/* 初始化语义分析器 */
void semantic_init(SemanticAnalyzer *analyzer);

/* 释放语义分析器 */
void semantic_free(SemanticAnalyzer *analyzer);

/* 分析 AST */
int semantic_analyze(SemanticAnalyzer *analyzer, ASTNode *ast);

/* 获取错误数 */
int semantic_error_count(SemanticAnalyzer *analyzer);

#endif /* SEMANTIC_H */
