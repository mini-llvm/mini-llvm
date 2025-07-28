int printf(const char *, ...);

int accumulate(const int a[], int n) {
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += a[i];
    }
    return sum;
}

int main(void) {
    int a[8] = {5, 4, 1, 3, 8, 7, 6, 2};
    printf("%d\n", accumulate(a, 8));
    return 0;
}
