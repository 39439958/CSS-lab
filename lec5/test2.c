#include <pthread.h>
#include <stdio.h>
#include <time.h>

void *start(void *a) {
    long sum = 0;
    long len = 1000;
    long start = (long)a * 1000 + 1;
    for (long i = start; i < start + len; i++) {
        sum += (i * i);
    }
    pthread_exit((void *)sum);
}

int main() {
    pthread_t tids[10];

    time_t t1, t2;
    time(&t1);
    for(long i = 0; i < 10; i++) {
        pthread_create(&tids[i], 0, start, (void *)i);
    }

    void *res1;
    long sum = 0;
    for(long i = 0; i < 10; i++) {
        pthread_join(tids[i], &res1);
        sum += (long)res1;
    }
    time(&t2);
    printf("main thread: %ld\n", sum);

    double cost = difftime(t2, t1);
    printf("cost : %f\n", cost);

    pthread_exit(0);
}