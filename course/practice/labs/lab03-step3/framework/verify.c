#include <stdio.h>
#include <string.h>

#include "student.h"
#include "verify.h"

static int g_failures = 0;

#define TEST_BEGIN(n) do { printf("[TEST %d] ", (n)); fflush(stdout); } while (0)
#define TEST_PASS() do { printf(" ... [PASS]\n"); } while (0)
#define TEST_FAIL(fmt, ...) do { printf(" ... [FAIL] " fmt "\n", ##__VA_ARGS__); g_failures++; } while (0)

static void test_primary(void) {
    TEST_BEGIN(1);
    StudentNode node = student_parse_primary("NUM", "42");
    if (node.type != NODE_NUM || strcmp(node.text, "42") != 0) TEST_FAIL("primary parse incorrect");
    else TEST_PASS();
}

static void test_if_node(void) {
    TEST_BEGIN(2);
    StudentNode node = student_parse_if("sum");
    if (node.type != NODE_IF || strcmp(node.text, "sum") != 0) TEST_FAIL("if node incorrect");
    else TEST_PASS();
}

static void test_func_node(void) {
    TEST_BEGIN(3);
    StudentNode node = student_parse_function("main");
    if (node.type != NODE_FUNC || strcmp(node.text, "main") != 0) TEST_FAIL("func node incorrect");
    else TEST_PASS();
}

int verify_run_all(void) {
    g_failures = 0;
    printf("==========================================\n");
    printf("    Lab03 - step_3 语法分析与 AST 验证    \n");
    printf("==========================================\n");
    test_primary();
    test_if_node();
    test_func_node();
    printf("==========================================\n");
    if (g_failures == 0) {
      printf("All tests passed!\n");
      return 0;
    }
    printf("%d test(s) FAILED.\n", g_failures);
    return 1;
}
