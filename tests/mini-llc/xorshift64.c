typedef unsigned long long uint64_t;

int printf(const char *, ...);

uint64_t xorshift64(uint64_t *x) {
    *x ^= *x << 13;
    *x ^= *x >> 7;
    *x ^= *x << 17;
    return *x;
}

void xorshift64_seed(uint64_t *x, uint64_t seed) {
    *x = seed;
}

int main(void) {
    uint64_t x;
    xorshift64_seed(&x, 123456789);
    printf("%llu\n", xorshift64(&x));
    printf("%llu\n", xorshift64(&x));
    printf("%llu\n", xorshift64(&x));
    printf("%llu\n", xorshift64(&x));
    printf("%llu\n", xorshift64(&x));
    return 0;
}
