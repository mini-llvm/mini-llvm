int printf(const char *, ...);

double mean(double *x, int n) {
    double sum = 0.;
    for (int i = 0; i < n; ++i) {
        sum += x[i];
    }
    return sum / n;
}

void linear_regression(double *x, double *y, int n, double *w, double *b) {
    double x_mean = mean(x, n);
    double y_mean = mean(y, n);
    double num = 0., den = 0.;
    for (int i = 0; i < n; ++i) {
        num += (x[i] - x_mean) * (y[i] - y_mean);
        den += (x[i] - x_mean) * (x[i] - x_mean);
    }
    *w = num / den;
    *b = y_mean - (*w) * x_mean;
}

int main(void) {
    double x[] = {
        1.9001917825078793,
        2.8675016470123094,
        0.24646406201049587,
        -4.8274080276724067,
        -2.364156058996004,
        1.0838045055670094,
        3.6077255443606546,
        -1.2144186081060946,
        -3.2890081854374631,
        -0.16997164804880782,
    };
    double y[] = {
        4.7666115459414575,
        6.6762014858415952,
        1.622918785946009,
        -8.5543129597970662,
        -3.7815421452152567,
        3.3633872468886068,
        8.2619128280700753,
        -1.4339020628079517,
        -5.5249158368294022,
        0.73830887774460174,
    };
    double w, b;
    linear_regression(x, y, 10, &w, &b);
    printf("%.17g %.17g\n", w, b);
    return 0;
}
