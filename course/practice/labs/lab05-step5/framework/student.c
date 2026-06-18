#include <stdio.h>
#include <string.h>

#include "student.h"
#include "verify.h"

int student_get_var_offset(const StudentVar *vars, int count, const char *name) {
    /* TODO(student): 在线性表里查变量偏移 */
    (void)vars;
    (void)count;
    (void)name;
    return 0;
}

void student_gen_expr(FILE *out, const char *kind, int value_or_offset) {
    /* TODO(student): 至少支持 NUM 和 IDENT 两种表达式 */
    (void)out;
    (void)kind;
    (void)value_or_offset;
}

void student_gen_stmt(FILE *out, const char *kind, int operand) {
    /* TODO(student): 至少支持 RETURN 语句 */
    (void)out;
    (void)kind;
    (void)operand;
}

int main(void) {
    return verify_run_all();
}
