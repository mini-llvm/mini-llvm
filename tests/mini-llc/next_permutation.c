int printf(const char *, ...);

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void reverse(int a[], int l, int r) {
    while (l < r) {
        swap(&a[l], &a[r]);
        ++l;
        --r;
    }
}

int next_permutation(int a[], int n) {
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) --i;
    if (i >= 0) {
        int j = n - 1;
        while (a[j] <= a[i]) --j;
        swap(&a[i], &a[j]);
    }
    reverse(a, i + 1, n - 1);
    return i >= 0;
}

int main(void) {
    int a[] = {1, 1, 2, 3};

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("%d\n", next_permutation(a, 4));
    printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);

    return 0;
}
