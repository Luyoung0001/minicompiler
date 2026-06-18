#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "student.h"
#include "verify.h"

static int g_failures = 0;

#define TEST_BEGIN(n) do { printf("[TEST %d] ", (n)); fflush(stdout); } while (0)
#define TEST_PASS() do { printf(" ... [PASS]\n"); } while (0)
#define TEST_FAIL(fmt, ...) do { printf(" ... [FAIL] " fmt "\n", ##__VA_ARGS__); g_failures++; } while (0)

static void test_parse_args(void) {
    TEST_BEGIN(1);
    char *argv[] = {"lab", "-o", "out.s", "input.c"};
    StudentConfig cfg = {0};
    if (student_parse_args(4, argv, &cfg) != 0) {
        TEST_FAIL("parse_args returned error");
        return;
    }
    if (!cfg.input_file || !cfg.output_file || strcmp(cfg.input_file, "input.c") != 0 || strcmp(cfg.output_file, "out.s") != 0) {
        TEST_FAIL("parsed config incorrect");
        return;
    }
    TEST_PASS();
}

static void test_read_file(void) {
    TEST_BEGIN(2);
    FILE *fp = fopen("tmp_input.c", "w");
    fputs("int main(){return 0;}\n", fp);
    fclose(fp);

    char *buf = student_read_file("tmp_input.c");
    if (!buf) {
        TEST_FAIL("read_file returned NULL");
        return;
    }
    if (strstr(buf, "return 0") == NULL) {
        TEST_FAIL("file content mismatch");
        free(buf);
        return;
    }
    free(buf);
    TEST_PASS();
}

static void test_emit_stub(void) {
    TEST_BEGIN(3);
    FILE *fp = fopen("tmp_out.s", "w");
    if (!fp) {
        TEST_FAIL("cannot create tmp_out.s");
        return;
    }
    if (student_emit_stub_program(fp) != 0) {
        fclose(fp);
        TEST_FAIL("emit function returned error");
        return;
    }
    fclose(fp);

    fp = fopen("tmp_out.s", "r");
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *buf = malloc(size + 1);
    fread(buf, 1, size, fp);
    buf[size] = '\0';
    fclose(fp);

    if (strstr(buf, "_start") == NULL || strstr(buf, "li a7") == NULL) {
        TEST_FAIL("assembly skeleton missing required markers");
        free(buf);
        return;
    }
    free(buf);
    TEST_PASS();
}

int verify_run_all(void) {
    g_failures = 0;
    printf("==========================================\n");
    printf("      Lab01 - step_1 最小驱动 验证       \n");
    printf("==========================================\n");
    test_parse_args();
    test_read_file();
    test_emit_stub();
    printf("==========================================\n");
    if (g_failures == 0) {
        printf("All tests passed!\n");
        return 0;
    }
    printf("%d test(s) FAILED.\n", g_failures);
    return 1;
}
