#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "student.h"
#include "verify.h"

void student_symtab_add(StudentScope *scope, const char *name) {
    /* TODO(student): 头插法加入符号 */
    (void)scope;
    (void)name;
}

int student_symtab_lookup(StudentScope *scope, const char *name) {
    /* TODO(student): 在链表里查名字 */
    (void)scope;
    (void)name;
    return 0;
}

int student_semantic_analyze(StudentScope *scope, const char **decls, int decl_count, const char **uses, int use_count) {
    /* TODO(student): 重复定义 + 未定义使用，返回错误个数 */
    (void)scope;
    (void)decls;
    (void)decl_count;
    (void)uses;
    (void)use_count;
    return -1;
}

int main(void) {
    return verify_run_all();
}
