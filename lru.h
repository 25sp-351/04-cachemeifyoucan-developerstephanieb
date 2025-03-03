#ifndef LRU_H
#define LRU_H

#include <stdlib.h>
#include <stdio.h>
#include "vec.h"

typedef struct LRUNode {
    int key;
    struct LRUNode *prev, *next;
} LRUNode;

typedef struct LRUCache {
    size_t capacity;
    size_t size;
    LRUNode *head, *tail;
    LRUNode **hash_table;
} LRUCache;

LRUCache *lru_create(size_t capacity);
void lru_access(LRUCache *cache, int key);
void lru_remove(LRUCache *cache, int key);
void lru_free(LRUCache *cache);
void lru_print(LRUCache *cache);

#endif