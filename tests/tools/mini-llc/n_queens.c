int putchar(int);

void print_board(int n, const int board[]) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (board[i] == j) {
                putchar('Q');
            } else {
                putchar('.');
            }
        }
        putchar('\n');
    }
    putchar('\n');
}

void n_queens(int n, int i, int board[], int cols[], int diags1[], int diags2[]) {
    if (i == n) {
        print_board(n, board);
        return;
    }
    for (int j = 0; j < n; ++j) {
        if (!cols[j] && !diags1[i - j + n - 1] && !diags2[i + j]) {
            board[i] = j;
            cols[j] = 1;
            diags1[i - j + n - 1] = 1;
            diags2[i + j] = 1;
            n_queens(n, i + 1, board, cols, diags1, diags2);
            cols[j] = 0;
            diags1[i - j + n - 1] = 0;
            diags2[i + j] = 0;
        }
    }
}

int main(void) {
    int board[8];
    int cols[8] = {0}, diags1[15] = {0}, diags2[15] = {0};
    n_queens(8, 0, board, cols, diags1, diags2);
    return 0;
}
