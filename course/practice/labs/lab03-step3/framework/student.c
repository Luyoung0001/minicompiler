#include <stdio.h>
#include <string.h>

#include "student.h"
#include "verify.h"

StudentNode student_parse_primary(const char *token_kind, const char *text) {
    /* TODO(student): NUM -> NODE_NUM, IDENT -> NODE_IDENT */
    StudentNode node = {NODE_UNKNOWN, {0}};
    (void)token_kind;
    (void)text;
    return node;
}

StudentNode student_parse_if(const char *cond_name) {
    /* TODO(student): 构造一个 IF 节点，并保存条件名字 */
    StudentNode node = {NODE_UNKNOWN, {0}};
    (void)cond_name;
    return node;
}

StudentNode student_parse_function(const char *func_name) {
    /* TODO(student): 构造一个函数定义节点 */
    StudentNode node = {NODE_UNKNOWN, {0}};
    (void)func_name;
    return node;
}

int main(void) {
    return verify_run_all();
}
