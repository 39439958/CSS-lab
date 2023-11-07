#include <stdio.h>
#include <stdlib.h>
#include "lf-stack.h"

#define TODO()\
do{\
    extern int printf(char *, ...);\
    printf("Add your code here: file %s, line %d\n", __FILE__, __LINE__);\
}while(0)




void lf_stack_init(lf_stack_t *stack){
    atomic_store(&stack->top, 0);
    stack->size = 0;
    return;
}

void lf_stack_push(lf_stack_t *stack, int value){
    // Exercise 1: lock-free stack.
    // Add your code here:
    Node_t *newNode = (Node_t *)malloc(sizeof(Node_t));
    newNode->value = value;
    
    while (1) {
      Node_t *old_top = atomic_load(&stack->top);
      newNode->next = old_top;
      if (atomic_compare_exchange_strong(&stack->top, &old_top, newNode)) {
        stack->size++;
        break;
      }
    }
}

int lf_stack_pop(lf_stack_t *stack){
    // Exercise 1: lock-free stack
    // Add your code here:
    int value = -1;
    while (1) {
      Node_t *old_top = atomic_load(&stack->top);
      if (old_top == 0)
        return value;
      Node_t *top_next = old_top->next;
      if (atomic_compare_exchange_strong(&stack->top, &old_top, top_next)) {
        stack->size--;
        value = old_top->value;
        break;
      }
    }
    return value;
}



int lf_stack_size(lf_stack_t *stack){
    // Exercise 1: lock-free stack
    // Add your code here:
    return stack->size;
}

