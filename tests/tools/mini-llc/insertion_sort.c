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
    int a[8] = {5, 4, 1, 3, 8, 7, 6, 2};
    insertion_sort(a, 8);
    printf("%d %d %d %d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);
    return 0;
}
