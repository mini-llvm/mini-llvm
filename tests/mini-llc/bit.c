int printf(const char *, ...);

int popcount1(unsigned x) {
    int count = 0;
    while (x) {
        count += x & 1;
        x >>= 1;
    }
    return count;
}

int popcount2(unsigned x) {
    int count = 0;
    while (x) {
        ++count;
        x &= x - 1;
    }
    return count;
}

int clz1(unsigned x) {
    if (x == 0) {
        return 32;
    }
    int count = 0;
    while (((x >> 31) & 1) == 0) {
        ++count;
        x <<= 1;
    }
    return count;
}

int clz2(unsigned x) {
    if (x == 0) {
        return 32;
    }
    int n = 0;
    if ((x & 0xffff0000) == 0) {
        n += 16;
        x <<= 16;
    }
    if ((x & 0xff000000) == 0) {
        n += 8;
        x <<= 8;
    }
    if ((x & 0xf0000000) == 0) {
        n += 4;
        x <<= 4;
    }
    if ((x & 0xc0000000) == 0) {
        n += 2;
        x <<= 2;
    }
    if ((x & 0x80000000) == 0) {
        n += 1;
    }
    return n;
}

int ctz1(unsigned x) {
    if (x == 0) {
        return 32;
    }
    int count = 0;
    while ((x & 1) == 0) {
        ++count;
        x >>= 1;
    }
    return count;
}

int ctz2(unsigned x) {
    if (x == 0) {
        return 32;
    }
    int n = 0;
    if ((x & 0x0000ffff) == 0) {
        n += 16;
        x >>= 16;
    }
    if ((x & 0x000000ff) == 0) {
        n += 8;
        x >>= 8;
    }
    if ((x & 0x0000000f) == 0) {
        n += 4;
        x >>= 4;
    }
    if ((x & 0x00000003) == 0) {
        n += 2;
        x >>= 2;
    }
    if ((x & 0x00000001) == 0) {
        n += 1;
    }
    return n;
}

int main(void) {
    printf("%d\n", popcount1(0x00000000));
    printf("%d\n", popcount1(0xffffffff));
    printf("%d\n", popcount1(0x00000001));
    printf("%d\n", popcount1(0x80000000));
    printf("%d\n", popcount1(0x12345678));
    printf("%d\n", popcount1(0xaabbccdd));

    printf("%d\n", popcount2(0x00000000));
    printf("%d\n", popcount2(0xffffffff));
    printf("%d\n", popcount2(0x00000001));
    printf("%d\n", popcount2(0x80000000));
    printf("%d\n", popcount2(0x12345678));
    printf("%d\n", popcount2(0xaabbccdd));

    printf("%d\n", clz1(0x00000000));
    printf("%d\n", clz1(0x80000000));
    printf("%d\n", clz1(0x40000000));
    printf("%d\n", clz1(0x20000000));
    printf("%d\n", clz1(0x10000000));
    printf("%d\n", clz1(0x00000001));
    printf("%d\n", clz1(0x00000002));
    printf("%d\n", clz1(0x00000004));
    printf("%d\n", clz1(0x00000008));
    printf("%d\n", clz1(0xffffffff));
    printf("%d\n", clz1(0x0000ffff));
    printf("%d\n", clz1(0x01010101));
    printf("%d\n", clz1(0x00110011));

    printf("%d\n", clz2(0x00000000));
    printf("%d\n", clz2(0x80000000));
    printf("%d\n", clz2(0x40000000));
    printf("%d\n", clz2(0x20000000));
    printf("%d\n", clz2(0x10000000));
    printf("%d\n", clz2(0x00000001));
    printf("%d\n", clz2(0x00000002));
    printf("%d\n", clz2(0x00000004));
    printf("%d\n", clz2(0x00000008));
    printf("%d\n", clz2(0xffffffff));
    printf("%d\n", clz2(0x0000ffff));
    printf("%d\n", clz2(0x01010101));
    printf("%d\n", clz2(0x00110011));

    printf("%d\n", ctz1(0x00000000));
    printf("%d\n", ctz1(0x00000001));
    printf("%d\n", ctz1(0x00000002));
    printf("%d\n", ctz1(0x00000004));
    printf("%d\n", ctz1(0x00000008));
    printf("%d\n", ctz1(0x80000000));
    printf("%d\n", ctz1(0x40000000));
    printf("%d\n", ctz1(0x20000000));
    printf("%d\n", ctz1(0x10000000));
    printf("%d\n", ctz1(0xffffffff));
    printf("%d\n", ctz1(0xffff0000));
    printf("%d\n", ctz1(0x10101010));
    printf("%d\n", ctz1(0x11001100));

    printf("%d\n", ctz2(0x00000000));
    printf("%d\n", ctz2(0x00000001));
    printf("%d\n", ctz2(0x00000002));
    printf("%d\n", ctz2(0x00000004));
    printf("%d\n", ctz2(0x00000008));
    printf("%d\n", ctz2(0x80000000));
    printf("%d\n", ctz2(0x40000000));
    printf("%d\n", ctz2(0x20000000));
    printf("%d\n", ctz2(0x10000000));
    printf("%d\n", ctz2(0xffffffff));
    printf("%d\n", ctz2(0xffff0000));
    printf("%d\n", ctz2(0x10101010));
    printf("%d\n", ctz2(0x11001100));

    return 0;
}
