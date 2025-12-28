int printf(const char *, ...);

void move(int disk, char from, char to) {
    printf("%d %c %c\n", disk, from, to);
}

void hanoi(int n, char from, char to, char aux) {
    if (n == 1) {
        move(1, from, to);
        return;
    }

    hanoi(n - 1, from, aux, to);
    move(n, from, to);
    hanoi(n - 1, aux, to, from);
}

int main(void) {
    hanoi(6, 'A', 'B', 'C');
    return 0;
}
