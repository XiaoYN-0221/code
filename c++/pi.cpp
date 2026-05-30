#include <cstdio>
#include <cstring>

// Classic spigot algorithm (Rabinowitz & Wagon, 1995)
// Streams decimal digits of pi one by one
int main() {
    const int LEN = 100000;
    int n = LEN * 10 / 3;
    int* a = new int[n];

    while (true) {
        for (int i = 0; i < n; i++) a[i] = 2;

    int nines = 0, predigit = 0;
    bool first = true;

    for (int j = 0; j < LEN; j++) {
        int q = 0;
        for (int i = n - 1; i >= 0; i--) {
            int x = 10 * a[i] + q * (i + 1);
            a[i] = x % (2 * i + 1);
            q = x / (2 * i + 1);
        }
        a[0] = q % 10;
        q /= 10;

        if (q == 9) {
            nines++;
        } else if (q == 10) {
            if (!first) printf("%d", predigit + 1);
            for (int k = 0; k < nines; k++) printf("0");
            predigit = 0;
            nines = 0;
            first = false;
        } else {
            if (!first) printf("%d", predigit);
            predigit = q;
            for (int k = 0; k < nines; k++) printf("9");
            nines = 0;
            first = false;
        }
        fflush(stdout);
    }
    printf("%d", predigit);
    fflush(stdout);
    }
    delete[] a;
    return 0;
}
