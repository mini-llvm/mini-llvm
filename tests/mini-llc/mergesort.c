int printf(const char *, ...);

void mergesort(int a[], int t[], int l, int r) {
    if (l == r) return;

    int mid = (l + r) / 2;
    mergesort(a, t, l, mid);
    mergesort(a, t, mid + 1, r);

    int i = l, j = mid + 1, k = l;
    while (i <= mid && j <= r) {
        if (a[i] <= a[j]) t[k++] = a[i++];
        else t[k++] = a[j++];
    }
    while (i <= mid) t[k++] = a[i++];
    while (j <= r) t[k++] = a[j++];

    for (k = l; k <= r; ++k) a[k] = t[k];
}

int main(void) {
    int a[16] = {7, 13, 11, 10, 5, 12, 0, 8, 14, 9, 6, 2, 3, 15, 1, 4};
    int t[16];
    mergesort(a, t, 0, 15);
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
