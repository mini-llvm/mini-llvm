int printf(const char *, ...);

void insertion_sort(int a[], int n) {
    for (int i = 1; i < n; ++i) {
        int tmp = a[i], j = i;
        while (j > 0 && a[j - 1] > tmp) {
            a[j] = a[j - 1];
            --j;
        }
        a[j] = tmp;
    }
}

int main(void) {
    int a[16] = {7, 13, 11, 10, 5, 12, 0, 8, 14, 9, 6, 2, 3, 15, 1, 4};
    insertion_sort(a, 16);
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
