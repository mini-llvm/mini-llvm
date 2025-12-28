int printf(const char *, ...);

int min(int x, int y) {
    return x < y ? x : y;
}

void floyd_warshall(int *dist, int n) {
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                dist[i * n + j] = min(dist[i * n + j], dist[i * n + k] + dist[k * n + j]);
            }
        }
    }
}

int main(void) {
    int dist[] = {
        0, 3, 8, 999, -4,
        999, 0, 999, 1, 7,
        999, 4, 0, 999, 999,
        2, 999, -5, 0, 999,
        999, 999, 999, 6, 0,
    };
    floyd_warshall(dist, 5);
    printf("%d %d %d %d %d\n", dist[0], dist[1], dist[2], dist[3], dist[4]);
    printf("%d %d %d %d %d\n", dist[5], dist[6], dist[7], dist[8], dist[9]);
    printf("%d %d %d %d %d\n", dist[10], dist[11], dist[12], dist[13], dist[14]);
    printf("%d %d %d %d %d\n", dist[15], dist[16], dist[17], dist[18], dist[19]);
    printf("%d %d %d %d %d\n", dist[20], dist[21], dist[22], dist[23], dist[24]);
    return 0;
}
