int printf(const char *, ...);

int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

int main(void) {
    printf("%d\n", gcd(1, 0));
    printf("%d\n", gcd(30, 42));
    printf("%d\n", gcd(2136, 3456));
    return 0;
}
