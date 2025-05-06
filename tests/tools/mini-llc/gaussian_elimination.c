double fabs(double);
int printf(const char *, ...);

void swap(double *a, double *b) {
    double t = *a;
    *a = *b;
    *b = t;
}

void gaussian_elimination(double A[], int m, int n) {
    int i = 0, j = 0;
    while (i < m && j < n) {
        int pivot = i;
        for (int k = i + 1; k < m; ++k) {
            if (fabs(A[k * n + j]) > fabs(A[pivot * n + j])) {
                pivot = k;
            }
        }
        for (int k = j; k < n; ++k) {
            swap(&A[i * n + k], &A[pivot * n + k]);
        }
        if (fabs(A[i * n + j]) > 1e-8) {
            double t = A[i * n + j];
            for (int k = j; k < n; ++k) {
                A[i * n + k] /= t;
            }
            for (int k = 0; k < m; ++k) {
                if (k != i) {
                    t = A[k * n + j];
                    for (int l = j; l < n; ++l) {
                        A[k * n + l] -= A[i* n + l] * t;
                    }
                }
            }
            ++i;
            ++j;
        } else {
            ++j;
        }
    }
}

int main(void) {
    double A[] = {2., 1., -1., 8., -3., -1., 2., -11., -2., 1., 2., -3.};
    gaussian_elimination(A, 3, 4);
    printf("%.17g %.17g %.17g\n", A[3], A[7], A[11]);
    return 0;
}
