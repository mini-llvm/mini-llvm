void *malloc(unsigned long);
void free(void *);
void *memset(void *, int, unsigned long);
int putchar(int);

int count_neighbors(unsigned char grid[], int m, int n, int x, int  y) {
    int count = 0;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            int xx = (x + dx + m) % m;
            int yy = (y + dy + n) % n;
            count += grid[xx * n + yy];
        }
    }
    return count;
}

void step(unsigned char grid[], unsigned char new_grid[], int m, int n) {
    for (int x = 0; x < m; ++x) {
        for (int y = 0; y < n; ++y) {
            int count = count_neighbors(grid, m, n, x, y);
            new_grid[x * n + y] = (grid[x * n + y] && (count == 2 || count == 3)) || (!grid[x * n + y] && count == 3);
        }
    }
}

void draw(unsigned char grid[], int m, int n) {
    for (int x = 0; x < m; ++x) {
        for (int y = 0; y < n; ++y) {
            putchar(grid[x * n + y] ? 'X' : '.');
        }
        putchar('\n');
    }
    putchar('\n');
}

void swap(unsigned char **a, unsigned char **b) {
    unsigned char *t = *a;
    *a = *b;
    *b = t;
}

int main(void) {
    const int M = 11, N = 38;

    unsigned char *grid = malloc(M * N * sizeof(unsigned char));
    unsigned char *new_grid = malloc(M * N * sizeof(unsigned char));

    memset(grid, 0, M * N * sizeof(unsigned char));

    grid[1 * N + 25] = 1;
    grid[2 * N + 23] = 1;
    grid[2 * N + 25] = 1;
    grid[3 * N + 13] = 1;
    grid[3 * N + 13] = 1;
    grid[3 * N + 14] = 1;
    grid[3 * N + 14] = 1;
    grid[3 * N + 21] = 1;
    grid[3 * N + 22] = 1;
    grid[3 * N + 35] = 1;
    grid[3 * N + 36] = 1;
    grid[4 * N + 12] = 1;
    grid[4 * N + 12] = 1;
    grid[4 * N + 16] = 1;
    grid[4 * N + 16] = 1;
    grid[4 * N + 21] = 1;
    grid[4 * N + 22] = 1;
    grid[4 * N + 35] = 1;
    grid[4 * N + 36] = 1;
    grid[5 * N + 1] = 1;
    grid[5 * N + 1] = 1;
    grid[5 * N + 2] = 1;
    grid[5 * N + 2] = 1;
    grid[5 * N + 11] = 1;
    grid[5 * N + 11] = 1;
    grid[5 * N + 17] = 1;
    grid[5 * N + 17] = 1;
    grid[5 * N + 21] = 1;
    grid[5 * N + 22] = 1;
    grid[6 * N + 1] = 1;
    grid[6 * N + 1] = 1;
    grid[6 * N + 2] = 1;
    grid[6 * N + 2] = 1;
    grid[6 * N + 11] = 1;
    grid[6 * N + 11] = 1;
    grid[6 * N + 15] = 1;
    grid[6 * N + 15] = 1;
    grid[6 * N + 17] = 1;
    grid[6 * N + 17] = 1;
    grid[6 * N + 18] = 1;
    grid[6 * N + 18] = 1;
    grid[6 * N + 23] = 1;
    grid[6 * N + 25] = 1;
    grid[7 * N + 11] = 1;
    grid[7 * N + 11] = 1;
    grid[7 * N + 17] = 1;
    grid[7 * N + 17] = 1;
    grid[7 * N + 25] = 1;
    grid[8 * N + 12] = 1;
    grid[8 * N + 12] = 1;
    grid[8 * N + 16] = 1;
    grid[8 * N + 16] = 1;
    grid[9 * N + 13] = 1;
    grid[9 * N + 14] = 1;

    draw(grid, M, N);
    step(grid, new_grid, M, N);
    swap(&grid, &new_grid);

    draw(grid, M, N);
    step(grid, new_grid, M, N);
    swap(&grid, &new_grid);

    draw(grid, M, N);
    step(grid, new_grid, M, N);
    swap(&grid, &new_grid);

    draw(grid, M, N);
    step(grid, new_grid, M, N);
    swap(&grid, &new_grid);

    draw(grid, M, N);
    step(grid, new_grid, M, N);
    swap(&grid, &new_grid);

    draw(grid, M, N);
    step(grid, new_grid, M, N);
    swap(&grid, &new_grid);

    draw(grid, M, N);
    step(grid, new_grid, M, N);
    swap(&grid, &new_grid);

    draw(grid, M, N);
    step(grid, new_grid, M, N);
    swap(&grid, &new_grid);

    draw(grid, M, N);
    step(grid, new_grid, M, N);
    swap(&grid, &new_grid);

    draw(grid, M, N);
    step(grid, new_grid, M, N);
    swap(&grid, &new_grid);

    free(grid);
    free(new_grid);
    return 0;
}
