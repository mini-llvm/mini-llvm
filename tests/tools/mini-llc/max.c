int printf(const char *, ...);

int max(int a, int b) {
    return a > b ? a : b;
}

int main(void) {
    printf("%d\n", max(42, 43));
    printf("%d\n", max(43, 42));
    printf("%d\n", max(42, -43));
    printf("%d\n", max(-43, 42));
    printf("%d\n", max(-42, -43));
    printf("%d\n", max(-43, -42));
    return 0;
}
