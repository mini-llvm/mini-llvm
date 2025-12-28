int printf(const char *, ...);

void matmul(int C[], const int A[], const int B[], int m, int n, int p) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            int sum = 0;
            for (int k = 0; k < p; ++k) {
                sum += A[i * p + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }
}

int main(void) {
    int A[6] = {3, 8, 2, 0, 4, 7};
    int B[12] = {6, 5, 2, 3, 1, 1, 7, 0, 7, 0, 8, 3};
    int C[8];
    matmul(C, A, B, 2, 4, 3);
    printf("%d %d %d %d %d %d %d %d\n", C[0], C[1], C[2], C[3], C[4], C[5], C[6], C[7]);
    return 0;
}
