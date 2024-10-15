int printf(const char *, ...);

void backtrack(int n, int x, int *count) {
    if (n == 0) {
        ++*count;
        return;
    }
    for (int y = x; y <= n; ++y) {
        backtrack(n - y, y, count);
    }
}

int partition(int n) {
    int count = 0;
    backtrack(n, 1, &count);
    return count;
}

int main(void) {
    printf("%d\n", partition(0));
    printf("%d\n", partition(1));
    printf("%d\n", partition(5));
    printf("%d\n", partition(10));
    printf("%d\n", partition(40));
    return 0;
}
