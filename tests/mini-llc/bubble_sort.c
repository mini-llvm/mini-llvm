int printf(const char *, ...);

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void bubble_sort(int a[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (a[j] > a[j + 1]) {
                swap(&a[j], &a[j + 1]);
            }
        }
    }
}

int main(void) {
    int a[16] = {7, 13, 11, 10, 5, 12, 0, 8, 14, 9, 6, 2, 3, 15, 1, 4};
    bubble_sort(a, 16);
    printf("%d\n", a[0]);
    printf("%d\n", a[1]);
    printf("%d\n", a[2]);
    printf("%d\n", a[3]);
    printf("%d\n", a[4]);
    printf("%d\n", a[5]);
    printf("%d\n", a[6]);
    printf("%d\n", a[7]);
    printf("%d\n", a[8]);
    printf("%d\n", a[9]);
    printf("%d\n", a[10]);
    printf("%d\n", a[11]);
    printf("%d\n", a[12]);
    printf("%d\n", a[13]);
    printf("%d\n", a[14]);
    printf("%d\n", a[15]);
    return 0;
}
