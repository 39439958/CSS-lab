#include <stdio.h>
#include <stdlib.h>
#include "lf-queue.h"

#define TODO()\
do{\
    extern int printf(char *, ...);\
    printf("Add your code here: file %s, line %d\n", __FILE__, __LINE__);\
}while(0)




void lf_queue_init(lf_queue_t *queue){
    node_t *node = (node_t *)calloc(1, sizeof(*node));
    atomic_store(&queue->front, node);
    atomic_store(&queue->rear, node);
    queue->size = 0;
    return;
}


void lf_queue_enqueue(lf_queue_t *queue, int value){
    // Exercise 2: fill in your code:
    // Add your code here:
    node_t *new = (node_t *)malloc(sizeof(node_t));
    new->value = value;

    while (1) {
      node_t *old_rear = atomic_load(&queue->rear);
      old_rear->next = new;
      if (atomic_compare_exchange_strong(&queue->rear, &old_rear, new))
        break;
    }
    queue->size++;
    return;
}

int lf_queue_dequeue(lf_queue_t *queue){
    // Exercise 2: fill in your code:
    // Add your code here:
    int value = -1;
    while (1) {
      node_t *old_front = atomic_load(&queue->front);
      node_t *old_rear = atomic_load(&queue->rear);
      node_t *front_next = old_front->next;
      if (old_front == old_rear)
        return value;
      if (atomic_compare_exchange_strong(&queue->front, &old_front, front_next)) {
        value = old_front->value;
        break;
      }
    }
    queue->size--;
    return value;
}

int lf_queue_size(lf_queue_t *queue){
    int size = 0;
    // Exercise 2: fill in your code:
    // Add your code here:
    size = queue->size;
    return size;    
}
