int printf(const char *, ...);

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void percolate_down(int heap[], int size, int i) {
    int tmp = heap[i];
    while (i < size / 2) {
        int j = i * 2 + 1;
        if (j + 1 < size && heap[j + 1] > heap[j]) ++j;
        if (tmp >= heap[j]) break;
        heap[i] = heap[j];
        i = j;
    }
    heap[i] = tmp;
}

void heapsort(int a[], int n) {
    for (int i = n / 2 - 1; i >= 0; --i) {
        percolate_down(a, n, i);
    }
    for (int i = n - 1; i >= 1; --i) {
        swap(&a[0], &a[i]);
        percolate_down(a, i, 0);
    }
}

int main(void) {
    int a[16] = {7, 13, 11, 10, 5, 12, 0, 8, 14, 9, 6, 2, 3, 15, 1, 4};
    heapsort(a, 16);
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
