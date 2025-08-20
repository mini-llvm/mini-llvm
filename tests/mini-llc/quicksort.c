int printf(const char *, ...);

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int a[], int l, int r) {
    int i = l - 1, j = r;
    for (;;) {
        do ++i; while (a[i] < a[r]);
        do --j; while (j >= l && a[j] > a[r]);
        if (i < j) swap(&a[i], &a[j]);
        else break;
    }
    swap(&a[i], &a[r]);
    return i;
}

void quicksort(int a[], int l, int r) {
    if (l < r) {
        int i = partition(a, l, r);
        quicksort(a, l, i - 1);
        quicksort(a, i + 1, r);
    }
}

int main(void) {
    int a[16] = {7, 13, 11, 10, 5, 12, 0, 8, 14, 9, 6, 2, 3, 15, 1, 4};
    quicksort(a, 0, 15);
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
