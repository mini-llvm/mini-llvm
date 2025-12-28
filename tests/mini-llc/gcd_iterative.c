int printf(const char *, ...);

int gcd(int a, int b) {
    while (b != 0) {
        int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

int main(void) {
    printf("%d\n", gcd(1, 0));
    printf("%d\n", gcd(30, 42));
    printf("%d\n", gcd(2136, 3456));
    return 0;
}
