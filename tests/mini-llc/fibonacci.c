int printf(const char *, ...);

int fibonacci(int n) {
    int f1 = 0, f2 = 1;
    for (int i = 1; i <= n; ++i) {
        int t = f1 + f2;
        f1 = f2;
        f2 = t;
    }
    return f1;
}

int main(void) {
    printf("%d\n", fibonacci(0));
    printf("%d\n", fibonacci(1));
    printf("%d\n", fibonacci(2));
    printf("%d\n", fibonacci(5));
    printf("%d\n", fibonacci(40));
    return 0;
}
