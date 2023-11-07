#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TODO()\
do{\
    extern int printf(char *, ...);\
    printf("Add your code here: file %s, line %d\n", __FILE__, __LINE__);\
}while(0)




#include "concur-hash.h"

// hash
/* this is the D. J. Bernstein hash function */
static unsigned int djb_hash(const char *cp) {
    unsigned hash = 5381;
    while(*cp)
        hash = 33 * hash ^ (unsigned char) *cp++;
    return hash % 128;
}

void hash_init(hash_t *ht, int num_buckets){
    pthread_mutex_init(&ht->mu, 0);
    ht->buckets = calloc(num_buckets, sizeof (*ht->buckets));
    ht->num_buckets = num_buckets;
}

void hash_insert(hash_t *ht, char *key, char *value) {
    // Exercise 2: complete the code below:
    // Add your code here:
    unsigned int index = djb_hash(key);
    node_t *newNode = (node_t *)malloc(sizeof(node_t));
    newNode->key = strdup(key);
    newNode->value = strdup(value);
    newNode->next = NULL;

    pthread_mutex_lock(&ht->mu);
    newNode->next = ht->buckets[index];
    ht->buckets[index] = newNode;
    ht->size++;
    pthread_mutex_unlock(&ht->mu);
}

char *hash_lookup(hash_t *ht, char *key) {
    // Exercise 2: complete the code below:
    // Add your code here:
    unsigned int index = djb_hash(key);
    pthread_mutex_lock(&ht->mu);

    node_t *p = ht->buckets[index];
    while (p != NULL) {
      if (strcmp(p->key, key) == 0) {
        pthread_mutex_unlock(&ht->mu);
        return p->value;
      }
      p = p->next;
    }
    pthread_mutex_unlock(&ht->mu);
    return NULL;
}

// return 0 for success; 1 for non-exiting "key"
int hash_delete(hash_t *ht, char *key){
    // Exercise 2: complete the code below:
    // Add your code here:
    unsigned int index = djb_hash(key);
    pthread_mutex_lock(&ht->mu);

    node_t *p = ht->buckets[index];
    node_t *prev = NULL;
    while (p != NULL && strcmp(p->key, key) == 0) {
      prev = p;
      p = p->next;
    }
    if (p == NULL) {
      pthread_mutex_unlock(&ht->mu);
      return 1;
    }
    if (prev == NULL) {
      ht->buckets[index] = p->next;
    } else {
      prev->next = p->next;
    }
    free(p);
    ht->size--;
    pthread_mutex_unlock(&ht->mu);
    return 0;
}

int hash_size(hash_t *ht){
    int size = 0;
    // Exercise 2: complete the code below:
    // Add your code here:
    size = ht->size;
    return size;
}
