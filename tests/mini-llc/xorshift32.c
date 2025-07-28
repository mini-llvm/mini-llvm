typedef unsigned uint32_t;

int printf(const char *, ...);

uint32_t xorshift32(uint32_t *x) {
    *x ^= *x << 13;
    *x ^= *x >> 17;
    *x ^= *x << 5;
    return *x;
}

void xorshift32_seed(uint32_t *x, uint32_t seed) {
    *x = seed;
}

int main(void) {
    uint32_t x;
    xorshift32_seed(&x, 123456789);
    printf("%u\n", xorshift32(&x));
    printf("%u\n", xorshift32(&x));
    printf("%u\n", xorshift32(&x));
    printf("%u\n", xorshift32(&x));
    printf("%u\n", xorshift32(&x));
    return 0;
}
