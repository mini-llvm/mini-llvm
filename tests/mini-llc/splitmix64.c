typedef unsigned long long uint64_t;

int printf(const char *, ...);

uint64_t splitmix64(uint64_t *x) {
    uint64_t z = (*x += 0x9e3779b97f4a7c15ULL);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    z = z ^ (z >> 31);
    *x = z;
    return *x;
}

void splitmix64_seed(uint64_t *x, uint64_t seed) {
    *x = seed;
}

int main(void) {
    uint64_t x;
    splitmix64_seed(&x, 123456789);
    printf("%llu\n", splitmix64(&x));
    printf("%llu\n", splitmix64(&x));
    printf("%llu\n", splitmix64(&x));
    printf("%llu\n", splitmix64(&x));
    printf("%llu\n", splitmix64(&x));
    return 0;
}
