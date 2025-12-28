void *malloc(unsigned long);
void free(void *);
int printf(const char *, ...);

int min(int a, int b, int c) {
   if (a < b && a < c) return a;
   else if (b < c) return b;
   else return c;
}

int edit_distance(const char *s, const char *t, int m, int n) {
    int *f[2];
    f[0] = malloc((n + 1) * sizeof(int));
    f[1] = malloc((n + 1) * sizeof(int));

    for (int j = 0; j <= n; ++j) {
        f[0][j] = j;
    }

    for (int i = 1; i <= m; ++i) {
        f[i % 2][0] = i;
        for (int j = 1; j <= n; ++j) {
            if (s[i - 1] == t[j - 1]) {
                f[i % 2][j] = f[(i - 1) % 2][j - 1];
            } else {
                f[i % 2][j] = min(f[(i - 1) % 2][j], f[i % 2][j - 1], f[(i - 1) % 2][j - 1]) + 1;
            }
        }
    }

    int result = f[m % 2][n];

    free(f[0]);
    free(f[1]);

    return result;
}

int main(void) {
    printf("%d\n", edit_distance("kitten", "sitting", 6, 7));
    printf("%d\n", edit_distance("intention", "execution", 9, 9));
    return 0;
}
