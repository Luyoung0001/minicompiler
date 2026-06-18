#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "student.h"
#include "verify.h"

static int g_failures = 0;

#define TEST_BEGIN(n) do { printf("[TEST %d] ", (n)); fflush(stdout); } while (0)
#define TEST_PASS() do { printf(" ... [PASS]\n"); } while (0)
#define TEST_FAIL(fmt, ...) do { printf(" ... [FAIL] " fmt "\n", ##__VA_ARGS__); g_failures++; } while (0)

static void test_offset(void) {
    TEST_BEGIN(1);
    StudentVar vars[] = {{"x", 12}, {"sum", 20}};
    if (student_get_var_offset(vars, 2, "sum") != 20) TEST_FAIL("offset lookup failed");
    else TEST_PASS();
}

static void test_expr(void) {
    TEST_BEGIN(2);
    FILE *fp = fopen("tmp_expr.s", "w");
    student_gen_expr(fp, "NUM", 42);
    fclose(fp);
    fp = fopen("tmp_expr.s", "r");
    char buf[256] = {0};
    fread(buf, 1, sizeof(buf) - 1, fp);
    fclose(fp);
    remove("tmp_expr.s");
    if (strstr(buf, "li a0, 42") == NULL) TEST_FAIL("NUM expr assembly missing");
    else TEST_PASS();
}

static void test_stmt(void) {
    TEST_BEGIN(3);
    FILE *fp = fopen("tmp_stmt.s", "w");
    student_gen_stmt(fp, "RETURN", 0);
    fclose(fp);
    fp = fopen("tmp_stmt.s", "r");
    char buf[256] = {0};
    fread(buf, 1, sizeof(buf) - 1, fp);
    fclose(fp);
    remove("tmp_stmt.s");
    if (strstr(buf, "ret") == NULL && strstr(buf, "j .Lret") == NULL) TEST_FAIL("RETURN stmt assembly missing");
    else TEST_PASS();
}

int verify_run_all(void) {
    g_failures = 0;
    printf("==========================================\n");
    printf("      Lab05 - step_5 代码生成 验证       \n");
    printf("==========================================\n");
    test_offset();
    test_expr();
    test_stmt();
    printf("==========================================\n");
    if (g_failures == 0) {
        printf("All tests passed!\n");
        return 0;
    }
    printf("%d test(s) FAILED.\n", g_failures);
    return 1;
}
