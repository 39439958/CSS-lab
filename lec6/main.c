#include <pthread.h>
#include <stdio.h>

int N =  10000;
int counter = 0;
pthread_mutex_t mu;

void *start(void *a) {
    pthread_mutex_lock(&mu);
    for (int i = 0; i < N; i++) {
        counter++;
    }
    pthread_mutex_unlock(&mu);
    pthread_exit(0);
}

int main() {
    pthread_t tid1, tid2;

    pthread_mutex_init(&mu, 0);
    pthread_create(&tid1, 0, start, 0);
    pthread_create(&tid2, 0, start, 0);
    
    pthread_join(tid1, 0);
    pthread_join(tid2, 0);

    printf("counter = %d\n", counter);
    
    pthread_exit(0);
}