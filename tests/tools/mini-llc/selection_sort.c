int printf(const char *, ...);

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void selection_sort(int a[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        int k = i;
        for (int j = i + 1; j < n; ++j) {
            if (a[j] < a[k]) k = j;
        }
        swap(&a[i], &a[k]);
    }
}

int main(void) {
    int a[8] = {5, 4, 1, 3, 8, 7, 6, 2};
    selection_sort(a, 8);
    printf("%d %d %d %d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);
    return 0;
}
