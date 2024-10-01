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
    int a[8] = {5, 4, 1, 3, 8, 7, 6, 2};
    int t[8];
    mergesort(a, t, 0, 7);
    printf("%d %d %d %d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);
    return 0;
}
