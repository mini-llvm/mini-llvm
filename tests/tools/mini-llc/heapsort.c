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
    int a[8] = {5, 4, 1, 3, 8, 7, 6, 2};
    heapsort(a, 8);
    printf("%d %d %d %d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);
    return 0;
}
