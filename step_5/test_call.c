/* test_call.c - 测试函数调用 */

int add(int a, int b) {
    return a + b;
}

int square(int x) {
    return x * x;
}

int main() {
    int result = add(3, 4);     /* 应该是 7 */
    int sq = square(5);         /* 应该是 25 */
    return add(result, sq);     /* 应该是 7 + 25 = 32 */
}
