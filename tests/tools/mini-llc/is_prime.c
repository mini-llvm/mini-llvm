int printf(const char *, ...);

int is_prime(int n) {
    if (n < 2) return 0;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return 0;
    }
    return 1;
}

int main(void) {
    printf("%d\n", is_prime(1));
    printf("%d\n", is_prime(2));
    printf("%d\n", is_prime(3));
    printf("%d\n", is_prime(4));
    printf("%d\n", is_prime(5));
    printf("%d\n", is_prime(6));
    printf("%d\n", is_prime(7));
    printf("%d\n", is_prime(8));
    printf("%d\n", is_prime(9));
    printf("%d\n", is_prime(10));
    printf("%d\n", is_prime(11));
    printf("%d\n", is_prime(12));
    printf("%d\n", is_prime(13));
    printf("%d\n", is_prime(14));
    printf("%d\n", is_prime(15));
    printf("%d\n", is_prime(16));
    printf("%d\n", is_prime(17));
    printf("%d\n", is_prime(18));
    printf("%d\n", is_prime(19));
    printf("%d\n", is_prime(20));
    return 0;
}
