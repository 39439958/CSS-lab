#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int N = atoll(argv[1]);
    int M = 0;
    for (int i = 1; i <= N; i++) {
        float x = 1.0 * rand() / RAND_MAX;
        float y = 1.0 * rand() / RAND_MAX;
        if (x*x + y*y <= 1.0)
            M++;
    }
    printf("pi = %f\n", 4.0 * M / N);
}