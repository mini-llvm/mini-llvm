int printf(const char *, ...);

void sieve_of_eratosthenes(int n, int is_prime[]) {
    is_prime[0] = 0;
    is_prime[1] = 0;
    for (int i = 2; i <= n; ++i) {
        is_prime[i] = 1;
    }
    for (int i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= n; j += i) {
                is_prime[j] = 0;
            }
        }
    }
}

int main(void) {
    int is_prime[21];
    sieve_of_eratosthenes(20, is_prime);
    printf("%d\n", is_prime[1]);
    printf("%d\n", is_prime[2]);
    printf("%d\n", is_prime[3]);
    printf("%d\n", is_prime[4]);
    printf("%d\n", is_prime[5]);
    printf("%d\n", is_prime[6]);
    printf("%d\n", is_prime[7]);
    printf("%d\n", is_prime[8]);
    printf("%d\n", is_prime[9]);
    printf("%d\n", is_prime[10]);
    printf("%d\n", is_prime[11]);
    printf("%d\n", is_prime[12]);
    printf("%d\n", is_prime[13]);
    printf("%d\n", is_prime[14]);
    printf("%d\n", is_prime[15]);
    printf("%d\n", is_prime[16]);
    printf("%d\n", is_prime[17]);
    printf("%d\n", is_prime[18]);
    printf("%d\n", is_prime[19]);
    printf("%d\n", is_prime[20]);
    return 0;
}
