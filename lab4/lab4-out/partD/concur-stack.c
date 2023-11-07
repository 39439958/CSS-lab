#include <stdio.h>
#include <stdlib.h>
#include "concur-stack.h"

#define TODO()\
do{\
    extern int printf(char *, ...);\
    printf("Add your code here: file %s, line %d\n", __FILE__, __LINE__);\
}while(0)




void concur_stack_init(concur_stack_t *stack){
    pthread_mutex_init(&stack->mu, NULL);
    stack->top = NULL;
    stack->size = 0;
    return;
}

void concur_stack_push(concur_stack_t *stack, int value){
    // Exercise 1: complete other operations of the concurrent stack.
    // Add your code here:
    node_t *p = (node_t *)malloc(sizeof(node_t));
    p->value = value;
    pthread_mutex_lock(&stack->mu);
    p->next = stack->top;
    stack->top = p;
    stack->size++;
    pthread_mutex_unlock(&stack->mu);
}

int concur_stack_pop(concur_stack_t *stack){
    int value = -1;
    // Exercise 1: complete other operations of the concurrent stack.
    // Add your code here:
    pthread_mutex_lock(&stack->mu);
    if (concur_stack_size(stack) == 0) {
      pthread_mutex_unlock(&stack->mu);
      return value;
    }
    node_t *p = stack->top;
    value = p->value;
    stack->top = p->next;
    stack->size--;
    free(p);
    pthread_mutex_unlock(&stack->mu);
    return value;
}

int concur_stack_size(concur_stack_t *stack){
    int size = 0;
    // Exercise 1: complete other operations of the concurrent stack.
    // Add your code here:
    size = stack->size;
    return size;
}
