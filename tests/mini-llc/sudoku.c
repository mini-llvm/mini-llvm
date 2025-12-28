int putchar(int);

void print_board(const int board[]) {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            putchar('0' + board[i * 9 + j]);
        }
        putchar('\n');
    }
}

int backtrack(int board[], int i, int j, int rows[], int cols[], int boxes[]) {
    if (i == 9) {
        return 1;
    }
    if (j == 9) {
        return backtrack(board, i + 1, 0, rows, cols, boxes);
    }
    if (board[i * 9 + j] != 0) {
        return backtrack(board, i, j + 1, rows, cols, boxes);
    }
    int mask = rows[i] | cols[j] | boxes[(i / 3) * 3 + (j / 3)];
    for (int k = 1; k <= 9; ++k) {
        if (!((mask >> (k - 1)) & 1)) {
            board[i * 9 + j] = k;
            rows[i] |= (1 << (k - 1));
            cols[j] |= (1 << (k - 1));
            boxes[(i / 3) * 3 + (j / 3)] |= (1 << (k - 1));
            if (backtrack(board, i, j + 1, rows, cols, boxes)) return 1;
            board[i * 9 + j] = 0;
            rows[i] &= ~(1 << (k - 1));
            cols[j] &= ~(1 << (k - 1));
            boxes[(i / 3) * 3 + (j / 3)] &= ~(1 << (k - 1));
        }
    }
    return 0;
}

int solve(int board[]) {
    int rows[9] = {0}, cols[9] = {0}, boxes[9] = {0};
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            int k = board[i * 9 + j];
            if (k != 0) {
                rows[i] |= (1 << (k - 1));
                cols[j] |= (1 << (k - 1));
                boxes[(i / 3) * 3 + (j / 3)] |= (1 << (k - 1));
            }
        }
    }
    return backtrack(board, 0, 0, rows, cols, boxes);
}

int main(void) {
    int board[81] = {
        5, 3, 0, 0, 7, 0, 0, 0, 0,
        6, 0, 0, 1, 9, 5, 0, 0, 0,
        0, 9, 8, 0, 0, 0, 0, 6, 0,
        8, 0, 0, 0, 6, 0, 0, 0, 3,
        4, 0, 0, 8, 0, 3, 0, 0, 1,
        7, 0, 0, 0, 2, 0, 0, 0, 6,
        0, 6, 0, 0, 0, 0, 2, 8, 0,
        0, 0, 0, 4, 1, 9, 0, 0, 5,
        0, 0, 0, 0, 8, 0, 0, 7, 9,
    };

    solve(board);
    print_board(board);

    return 0;
}
