#include <pthread.h>
#include <stdio.h>

void *start1(void *a) {
    printf("this is a thread%ld\n", (long)a);
    pthread_exit(a);
}

void *start2(void *a) {
    printf("this is a thread%ld\n", (long)a);
    pthread_exit(a);
}

void *start3(void *a) {
    printf("this is a thread%ld\n", (long)a);
    pthread_exit(a);
}

void *start4(void *a) {
    printf("this is a thread%ld\n", (long)a);
    pthread_exit(a);
}

int main() {
    pthread_t tid1, tid2, tid3, tid4;

    pthread_create(&tid1, 0, start1, (void *)0);
    pthread_create(&tid2, 0, start2, (void *)1);
    pthread_create(&tid3, 0, start3, (void *)2);
    pthread_create(&tid4, 0, start4, (void *)3);
    
    void *res1;
    pthread_join(tid1, &res1);
    printf("thread exited: %ld\n", (long)res1);
    void *res2;
    pthread_join(tid2, &res2);
    printf("thread exited: %ld\n", (long)res2);
    void *res3;
    pthread_join(tid3, &res3);
    printf("thread exited: %ld\n", (long)res3);
    void *res4;
    pthread_join(tid4, &res4);
    printf("thread exited: %ld\n", (long)res4);

    printf("main thread\n");
    pthread_exit(0);
}