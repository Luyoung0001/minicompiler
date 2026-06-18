#include <stdio.h>
#include <string.h>

#include "student.h"
#include "verify.h"

static int g_failures = 0;

#define TEST_BEGIN(n) do { printf("[TEST %d] ", (n)); fflush(stdout); } while (0)
#define TEST_PASS() do { printf(" ... [PASS]\n"); } while (0)
#define TEST_FAIL(fmt, ...) do { printf(" ... [FAIL] " fmt "\n", ##__VA_ARGS__); g_failures++; } while (0)

static void test_skip_ws(void) {
    TEST_BEGIN(1);
    StudentLexer lx;
    student_lexer_init(&lx, "   \n\tint x");
    student_skip_whitespace(&lx);
    if (*lx.pos != 'i') TEST_FAIL("expected lexer to stop at 'i'");
    else TEST_PASS();
}

static void test_ident_kw(void) {
    TEST_BEGIN(2);
    StudentLexer lx;
    student_lexer_init(&lx, "return sum");
    StudentToken tok = student_scan_identifier(&lx);
    if (tok.type != ST_TOK_RETURN) TEST_FAIL("return not recognized as keyword");
    else TEST_PASS();
}

static void test_number(void) {
    TEST_BEGIN(3);
    StudentLexer lx;
    student_lexer_init(&lx, "12345;");
    StudentToken tok = student_scan_number(&lx);
    if (tok.type != ST_TOK_NUM || strcmp(tok.value, "12345") != 0) TEST_FAIL("number token incorrect: %s", tok.value);
    else TEST_PASS();
}

int verify_run_all(void) {
    g_failures = 0;
    printf("==========================================\n");
    printf("      Lab02 - step_2 词法分析器 验证      \n");
    printf("==========================================\n");
    test_skip_ws();
    test_ident_kw();
    test_number();
    printf("==========================================\n");
    if (g_failures == 0) {
        printf("All tests passed!\n");
        return 0;
    }
    printf("%d test(s) FAILED.\n", g_failures);
    return 1;
}
