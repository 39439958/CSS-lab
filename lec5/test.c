#include <pthread.h>
#include <stdio.h>
#include <time.h>

void *start(void *a) {
    long n = (long)a;
    long r = n*n;
    pthread_exit((void *)r);
}

int main() {
    int N =  10000;
    pthread_t tids[N + 1];

    time_t t1, t2, t3;
    time(&t1);
    for(long i = 0; i <= N; i++) {
        pthread_create(&tids[i], 0, start, (void *)i);
    }

    void *res1;
    long sum = 0;
    for(long i = 0; i <= N; i++) {
        pthread_join(tids[i], &res1);
        sum += (long)res1;
    }

    printf("main thread: %ld\n", sum);
    time(&t2);

    sum = 0;
    for(long i = 0; i <= N; i++) {
        sum += (i * i);
    }

    printf("main thread: %ld\n", sum);
    time(&t3);

    double cost1 = difftime(t2, t1);
    double cost2 = difftime(t3, t2);

    printf("cost1 : %f\n", cost1);
    printf("cost2 : %f\n", cost2);

    pthread_exit(0);
}