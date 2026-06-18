#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "student.h"
#include "verify.h"

void student_lexer_init(StudentLexer *lexer, const char *source) {
    lexer->source = source;
    lexer->pos = source;
}

void student_skip_whitespace(StudentLexer *lexer) {
    /* TODO(student): 跳过空白字符 */
    (void)lexer;
}

StudentToken student_scan_identifier(StudentLexer *lexer) {
    /* TODO(student): 扫描标识符，并识别 int/if/return 关键字 */
    StudentToken tok = {ST_TOK_UNKNOWN, {0}};
    (void)lexer;
    return tok;
}

StudentToken student_scan_number(StudentLexer *lexer) {
    /* TODO(student): 扫描连续数字 */
    StudentToken tok = {ST_TOK_UNKNOWN, {0}};
    (void)lexer;
    return tok;
}

int main(void) {
    return verify_run_all();
}
