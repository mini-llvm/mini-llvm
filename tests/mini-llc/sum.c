int printf(const char *, ...);

int sum(int n) {
    int sum = 0;
    for (int i = 1; i <= n; ++i) {
        sum += i;
    }
    return sum;
}

int main(void) {
    printf("%d\n", sum(0));
    printf("%d\n", sum(1));
    printf("%d\n", sum(100));
    return 0;
}
