/*
 * symbol.c - 符号表实现
 * Step 4: 语义分析
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol.h"

/* 创建符号表 */
SymbolTable *symtab_new(SymbolTable *parent) {
    SymbolTable *table = calloc(1, sizeof(SymbolTable));
    table->symbols = NULL;
    table->scope_level = parent ? parent->scope_level + 1 : 0;
    table->stack_offset = parent ? parent->stack_offset : 0;
    table->parent = parent;
    return table;
}

/* 释放符号表 */
void symtab_free(SymbolTable *table) {
    if (!table) return;

    Symbol *sym = table->symbols;
    while (sym) {
        Symbol *next = sym->next;
        free(sym->name);
        free(sym);
        sym = next;
    }
    free(table);
}

/* 进入新作用域 */
SymbolTable *symtab_enter_scope(SymbolTable *current) {
    return symtab_new(current);
}

/* 离开作用域 */
SymbolTable *symtab_leave_scope(SymbolTable *current) {
    if (!current) return NULL;
    SymbolTable *parent = current->parent;
    symtab_free(current);
    return parent;
}

/* 添加符号 */
Symbol *symtab_add(SymbolTable *table, const char *name, SymbolKind kind, DataType type) {
    Symbol *sym = calloc(1, sizeof(Symbol));
    sym->name = strdup(name);
    sym->kind = kind;
    sym->type = type;
    sym->scope_level = table->scope_level;

    /* 为变量分配栈空间 */
    if (kind == SYM_VAR) {
        table->stack_offset += 4; /* 假设所有变量占 4 字节 */
        sym->offset = table->stack_offset;
    }

    /* 插入到链表头部 */
    sym->next = table->symbols;
    table->symbols = sym;

    return sym;
}

/* 查找符号 (当前作用域) */
Symbol *symtab_lookup_local(SymbolTable *table, const char *name) {
    if (!table) return NULL;

    for (Symbol *sym = table->symbols; sym; sym = sym->next) {
        if (strcmp(sym->name, name) == 0) {
            return sym;
        }
    }
    return NULL;
}

/* 查找符号 (包括父作用域) */
Symbol *symtab_lookup(SymbolTable *table, const char *name) {
    while (table) {
        Symbol *sym = symtab_lookup_local(table, name);
        if (sym) return sym;
        table = table->parent;
    }
    return NULL;
}

/* 类型名转换 */
const char *type_name(DataType type) {
    switch (type) {
        case TYPE_INT:  return "int";
        case TYPE_CHAR: return "char";
        case TYPE_VOID: return "void";
        default:        return "unknown";
    }
}

DataType type_from_name(const char *name) {
    if (strcmp(name, "int") == 0 || strcmp(name, "INT") == 0) return TYPE_INT;
    if (strcmp(name, "char") == 0 || strcmp(name, "CHAR") == 0) return TYPE_CHAR;
    if (strcmp(name, "void") == 0 || strcmp(name, "VOID") == 0) return TYPE_VOID;
    return TYPE_UNKNOWN;
}
