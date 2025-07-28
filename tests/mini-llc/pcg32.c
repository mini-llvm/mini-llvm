typedef unsigned uint32_t;
typedef unsigned long long uint64_t;

int printf(const char *, ...);

uint32_t pcg32(uint64_t *state, uint64_t inc) {
    uint64_t oldstate = *state;
    *state = oldstate * 6364136223846793005ULL + inc;
    uint32_t xorshifted = ((oldstate >> 18) ^ oldstate) >> 27;
    uint32_t rot = oldstate >> 59;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

void pcg32_seed(uint64_t *state, uint64_t *inc, uint64_t seed, uint64_t seq) {
    *state = 0ULL;
    *inc = (seq << 1) | 1ULL;
    pcg32(state, *inc);
    *state += seed;
    pcg32(state, *inc);
}

int main(void) {
    uint64_t state, inc;
    pcg32_seed(&state, &inc, 12345ULL, 1ULL);
    printf("%u\n", pcg32(&state, inc));
    printf("%u\n", pcg32(&state, inc));
    printf("%u\n", pcg32(&state, inc));
    printf("%u\n", pcg32(&state, inc));
    printf("%u\n", pcg32(&state, inc));
    return 0;
}
