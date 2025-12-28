void *calloc(unsigned long, unsigned long);
void free(void *);
int printf(const char *, ...);

typedef unsigned long long uint64_t;

uint64_t *bitset_create(int n) {
    return calloc((n + 63) / 64, 8);
}

void bitset_destroy(uint64_t *bitset) {
    free(bitset);
}

int bitset_test(uint64_t *bitset, int i) {
    return (bitset[i / 64] >> (i % 64)) & 1;
}

void bitset_set(uint64_t *bitset, int i) {
    bitset[i / 64] |= (1ULL << (i % 64));
}

void bitset_clear(uint64_t *bitset, int i) {
    bitset[i / 64] &= ~(1ULL << (i % 64));
}

int main(void) {
    uint64_t *bitset = bitset_create(256);

    bitset_set(bitset, 0);
    bitset_set(bitset, 32);
    bitset_set(bitset, 63);

    bitset_set(bitset, 64);
    bitset_set(bitset, 96);
    bitset_set(bitset, 127);

    bitset_set(bitset, 128);
    bitset_set(bitset, 160);
    bitset_set(bitset, 191);

    bitset_set(bitset, 192);
    bitset_set(bitset, 224);
    bitset_set(bitset, 255);

    printf("%d\n", bitset_test(bitset, 0));
    printf("%d\n", bitset_test(bitset, 1));

    printf("%d\n", bitset_test(bitset, 31));
    printf("%d\n", bitset_test(bitset, 32));
    printf("%d\n", bitset_test(bitset, 33));

    printf("%d\n", bitset_test(bitset, 62));
    printf("%d\n", bitset_test(bitset, 63));

    printf("%d\n", bitset_test(bitset, 64));
    printf("%d\n", bitset_test(bitset, 65));

    printf("%d\n", bitset_test(bitset, 95));
    printf("%d\n", bitset_test(bitset, 96));
    printf("%d\n", bitset_test(bitset, 97));

    printf("%d\n", bitset_test(bitset, 126));
    printf("%d\n", bitset_test(bitset, 127));

    printf("%d\n", bitset_test(bitset, 128));
    printf("%d\n", bitset_test(bitset, 129));

    printf("%d\n", bitset_test(bitset, 159));
    printf("%d\n", bitset_test(bitset, 160));
    printf("%d\n", bitset_test(bitset, 161));

    printf("%d\n", bitset_test(bitset, 190));
    printf("%d\n", bitset_test(bitset, 191));

    printf("%d\n", bitset_test(bitset, 192));
    printf("%d\n", bitset_test(bitset, 193));

    printf("%d\n", bitset_test(bitset, 223));
    printf("%d\n", bitset_test(bitset, 224));
    printf("%d\n", bitset_test(bitset, 225));

    printf("%d\n", bitset_test(bitset, 254));
    printf("%d\n", bitset_test(bitset, 255));

    bitset_clear(bitset, 64);
    bitset_clear(bitset, 160);
    bitset_clear(bitset, 255);

    printf("%d\n", bitset_test(bitset, 0));
    printf("%d\n", bitset_test(bitset, 1));

    printf("%d\n", bitset_test(bitset, 31));
    printf("%d\n", bitset_test(bitset, 32));
    printf("%d\n", bitset_test(bitset, 33));

    printf("%d\n", bitset_test(bitset, 62));
    printf("%d\n", bitset_test(bitset, 63));

    printf("%d\n", bitset_test(bitset, 64));
    printf("%d\n", bitset_test(bitset, 65));

    printf("%d\n", bitset_test(bitset, 95));
    printf("%d\n", bitset_test(bitset, 96));
    printf("%d\n", bitset_test(bitset, 97));

    printf("%d\n", bitset_test(bitset, 126));
    printf("%d\n", bitset_test(bitset, 127));

    printf("%d\n", bitset_test(bitset, 128));
    printf("%d\n", bitset_test(bitset, 129));

    printf("%d\n", bitset_test(bitset, 159));
    printf("%d\n", bitset_test(bitset, 160));
    printf("%d\n", bitset_test(bitset, 161));

    printf("%d\n", bitset_test(bitset, 190));
    printf("%d\n", bitset_test(bitset, 191));

    printf("%d\n", bitset_test(bitset, 192));
    printf("%d\n", bitset_test(bitset, 193));

    printf("%d\n", bitset_test(bitset, 223));
    printf("%d\n", bitset_test(bitset, 224));
    printf("%d\n", bitset_test(bitset, 225));

    printf("%d\n", bitset_test(bitset, 254));
    printf("%d\n", bitset_test(bitset, 255));

    bitset_destroy(bitset);
    return 0;
}
