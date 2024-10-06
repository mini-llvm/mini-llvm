int printf(const char *, ...);

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int a[], int l, int r) {
    int i = l, j = r - 1;
    for (;;) {
        while (a[i] < a[r]) ++i;
        while (j > l && a[j] > a[r]) --j;
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
    int a[8] = {5, 4, 1, 3, 8, 7, 6, 2};
    quicksort(a, 0, 7);
    printf("%d %d %d %d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);
    return 0;
}
