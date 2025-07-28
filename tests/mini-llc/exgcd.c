int printf(const char *, ...);

void exgcd(int a, int b, int *d, int *x, int *y) {
    if (b == 0) {
        *d = a;
        *x = 1;
        *y = 0;
    } else {
        exgcd(b, a % b, d, y, x);
        *y -= *x * (a / b);
    }
}

int main(void) {
    int d, x, y;
    exgcd(9, 9, &d, &x, &y);
    printf("%d %d %d\n", d, x, y);
    exgcd(30, 12, &d, &x, &y);
    printf("%d %d %d\n", d, x, y);
    exgcd(99, 78, &d, &x, &y);
    printf("%d %d %d\n", d, x, y);
    return 0;
}
