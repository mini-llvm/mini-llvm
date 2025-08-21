typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

int printf(const char *, ...);

const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void base64_encode(const uint8_t *data, int size, char *out) {
    int i, j, k;

    for (i = 0, j = 0; i < size;) {
        uint32_t octet1 = i < size ? data[i++] : 0;
        uint32_t octet2 = i < size ? data[i++] : 0;
        uint32_t octet3 = i < size ? data[i++] : 0;

        uint32_t triple = (octet1 << 16) | (octet2 << 8) | octet3;

        out[j++] = base64_table[(triple >> 18) & 0x3f];
        out[j++] = base64_table[(triple >> 12) & 0x3f];
        out[j++] = base64_table[(triple >> 6) & 0x3f];
        out[j++] = base64_table[triple & 0x3f];
    }

    for (k = 0; k < (3 - size % 3) % 3; ++k) {
        out[j - 1 - k] = '=';
    }

    out[j] = '\0';
}

int main(void) {
    char out[256];

    base64_encode((const uint8_t *)"", 0, out);
    printf("%s\n", out);

    base64_encode((const uint8_t *)"f", 1, out);
    printf("%s\n", out);

    base64_encode((const uint8_t *)"fo", 2, out);
    printf("%s\n", out);

    base64_encode((const uint8_t *)"foo", 3, out);
    printf("%s\n", out);

    base64_encode((const uint8_t *)"foob", 4, out);
    printf("%s\n", out);

    base64_encode((const uint8_t *)"fooba", 5, out);
    printf("%s\n", out);

    base64_encode((const uint8_t *)"foobar", 6, out);
    printf("%s\n", out);

    return 0;
}
