/* test_error.c - 语义分析测试 (错误代码) */

int main() {
    int x = 10;

    /* 错误1: 使用未定义的变量 */
    int sum = x + undefined_var;

    /* 错误2: 变量重复定义 */
    int x = 20;

    return 0;
}
