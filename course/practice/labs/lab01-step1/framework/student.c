#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "student.h"
#include "verify.h"

int student_parse_args(int argc, char **argv, StudentConfig *cfg) {
    /* TODO(student): 解析 -o 和输入文件 */
    (void)argc;
    (void)argv;
    (void)cfg;
    return -1;
}

char *student_read_file(const char *filename) {
    /* TODO(student): 读取整个文件并返回以 '\0' 结尾的缓冲区 */
    (void)filename;
    return NULL;
}

int student_emit_stub_program(FILE *out) {
    /* TODO(student): 写出最小 RISC-V 汇编骨架 */
    (void)out;
    return -1;
}

int main(void) {
    return verify_run_all();
}
