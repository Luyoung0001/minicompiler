/*
 * symbol.h - 符号表定义
 * Step 4: 语义分析
 */

#ifndef SYMBOL_H
#define SYMBOL_H

/* 符号类型 */
typedef enum {
    SYM_VAR,        /* 变量 */
    SYM_FUNC,       /* 函数 */
} SymbolKind;

/* 数据类型 */
typedef enum {
    TYPE_INT,
    TYPE_CHAR,
    TYPE_VOID,
    TYPE_UNKNOWN,
} DataType;

/* 符号 */
typedef struct Symbol {
    char *name;             /* 符号名 */
    SymbolKind kind;        /* 符号类型 */
    DataType type;          /* 数据类型 */
    int scope_level;        /* 作用域层级 */
    int offset;             /* 栈偏移 (用于代码生成) */
    struct Symbol *next;    /* 链表下一个 */
} Symbol;

/* 符号表 (支持作用域) */
typedef struct SymbolTable {
    Symbol *symbols;            /* 符号链表 */
    int scope_level;            /* 当前作用域层级 */
    int stack_offset;           /* 当前栈偏移 */
    struct SymbolTable *parent; /* 父作用域 */
} SymbolTable;

/* 创建符号表 */
SymbolTable *symtab_new(SymbolTable *parent);

/* 释放符号表 */
void symtab_free(SymbolTable *table);

/* 进入新作用域 */
SymbolTable *symtab_enter_scope(SymbolTable *current);

/* 离开作用域 */
SymbolTable *symtab_leave_scope(SymbolTable *current);

/* 添加符号 */
Symbol *symtab_add(SymbolTable *table, const char *name, SymbolKind kind, DataType type);

/* 查找符号 (当前作用域) */
Symbol *symtab_lookup_local(SymbolTable *table, const char *name);

/* 查找符号 (包括父作用域) */
Symbol *symtab_lookup(SymbolTable *table, const char *name);

/* 类型名转换 */
const char *type_name(DataType type);
DataType type_from_name(const char *name);

#endif /* SYMBOL_H */
