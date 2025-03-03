#ifndef LFU_H
#define LFU_H

#include <stdlib.h>
#include <stdio.h>
#include "vec.h"

typedef struct LFUNode {
    int key;
    int frequency;
    struct LFUNode *prev, *next;
} LFUNode;

typedef struct LFUCache {
    size_t capacity;
    size_t size;
    LFUNode *head, *tail;
    LFUNode **hash_table;
} LFUCache;

LFUCache *lfu_create(size_t capacity);
void lfu_access(LFUCache *cache, int key);
void lfu_remove(LFUCache *cache, int key);
void lfu_free(LFUCache *cache);
void lfu_print(LFUCache *cache);

#endif