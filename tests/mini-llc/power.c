int printf(const char *, ...);

int power(int base, int exp) {
    int result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

int main(void) {
    printf("%d\n", power(2, 0));
    printf("%d\n", power(3, 5));
    printf("%d\n", power(4, 10));
    return 0;
}
