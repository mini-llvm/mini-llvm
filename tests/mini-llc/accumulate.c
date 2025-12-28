int printf(const char *, ...);

int accumulate(const int a[], int n) {
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += a[i];
    }
    return sum;
}

int main(void) {
    int a[16] = {7, 13, 11, 10, 5, 12, 0, 8, 14, 9, 6, 2, 3, 15, 1, 4};
    printf("%d\n", accumulate(a, 16));
    return 0;
}
