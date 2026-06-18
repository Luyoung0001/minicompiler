#include <stdio.h>
#include <stdlib.h>

#include "student.h"
#include "verify.h"

static int g_failures = 0;

#define TEST_BEGIN(n) do { printf("[TEST %d] ", (n)); fflush(stdout); } while (0)
#define TEST_PASS() do { printf(" ... [PASS]\n"); } while (0)
#define TEST_FAIL(fmt, ...) do { printf(" ... [FAIL] " fmt "\n", ##__VA_ARGS__); g_failures++; } while (0)

static void free_scope(StudentScope *scope) {
    StudentSymbol *sym = scope->head;
    while (sym) {
        StudentSymbol *next = sym->next;
        free(sym);
        sym = next;
    }
}

static void test_add_lookup(void) {
    TEST_BEGIN(1);
    StudentScope scope = {0};
    student_symtab_add(&scope, "x");
    if (!student_symtab_lookup(&scope, "x")) TEST_FAIL("lookup failed");
    else TEST_PASS();
    free_scope(&scope);
}

static void test_undefined(void) {
    TEST_BEGIN(2);
    StudentScope scope = {0};
    const char *decls[] = {"x"};
    const char *uses[] = {"x", "y"};
    int errors = student_semantic_analyze(&scope, decls, 1, uses, 2);
    if (errors != 1) TEST_FAIL("expected 1 undefined error, got %d", errors);
    else TEST_PASS();
    free_scope(&scope);
}

static void test_redefine(void) {
    TEST_BEGIN(3);
    StudentScope scope = {0};
    const char *decls[] = {"x", "x"};
    int errors = student_semantic_analyze(&scope, decls, 2, NULL, 0);
    if (errors != 1) TEST_FAIL("expected 1 redefine error, got %d", errors);
    else TEST_PASS();
    free_scope(&scope);
}

int verify_run_all(void) {
    g_failures = 0;
    printf("==========================================\n");
    printf("      Lab04 - step_4 语义分析 验证       \n");
    printf("==========================================\n");
    test_add_lookup();
    test_undefined();
    test_redefine();
    printf("==========================================\n");
    if (g_failures == 0) {
        printf("All tests passed!\n");
        return 0;
    }
    printf("%d test(s) FAILED.\n", g_failures);
    return 1;
}
