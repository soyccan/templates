#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXN 100000005

char Q[MAXN];
int U[MAXN];

int reciprocal(int x, char q[], int last[]) {
    // output 1/x in repeating decimal representation
    // return length of its repeating period

    int i = 0;
    int r = 1; // remainder
    bool sign = false;

    if (x == 1) { printf("1\n"); return 0; }
    if (x == 0) return -1;
    if (x < 0) sign = true, x = -x;

    memset(last, -1, x * sizeof(int));
    while (i < MAXN) {
        q[i] = r / x;
        r %= x;

        // printf("q:");
        // for (int j=0; j<30; j++) printf("%d",q[j]);
        // printf("\nr:%d", r);
        // printf( "\n\n");

        if (last[r] != -1 || r == 0) {
            if (sign) printf("-");
            printf("0.");
            for (int j=1; j<=i; j++) {
                if (r != 0 && j == last[r] + 1)
                    printf("(");
                printf("%d", q[j]);
            }
            if (r != 0) printf(")");
            printf("\n");
            return i;
        }
        else last[r] = i;

        r *= 10;
        i++;
    }
    return -1;
}

int main() {
    for (int x=1; x<=100; x++) {
        printf("%d ", x);
        printf("%d\n", reciprocal(x, Q, U));
    }
    return 0;
}
