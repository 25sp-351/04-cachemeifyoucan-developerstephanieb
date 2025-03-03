#include "lfu.h"

LFUCache *lfu_create(size_t capacity) {
  LFUCache *cache = malloc(sizeof(LFUCache));
  cache->capacity = capacity;
  cache->size = 0;
  cache->head = NULL;
  cache->tail = NULL;
  cache->hash_table = calloc(capacity, sizeof(LFUNode *));
  return cache;
}

void lfu_update_frequency(LFUCache *cache, LFUNode *node) {
  node->frequency++;

  while (node->prev && node->prev->frequency <= node->frequency) {
    LFUNode *prev = node->prev;
    LFUNode *next = node->next;

    if (prev->prev)
      prev->prev->next = node;
    node->prev = prev->prev;
    prev->next = next;
    node->next = prev;
    prev->prev = node;

    if (!node->prev)
      cache->head = node;
    if (!prev->next)
      cache->tail = prev;
  }
}

void lfu_remove_lfu(LFUCache *cache) {
  if (!cache->tail)
    return;

  LFUNode *lfu = cache->tail;
  if (lfu->prev)
    lfu->prev->next = NULL;
  else
    cache->head = NULL;

  cache->tail = lfu->prev;
  free(lfu);
  cache->size--;
}

void lfu_access(LFUCache *cache, int key) {
  if (cache->hash_table[key]) {
    lfu_update_frequency(cache, cache->hash_table[key]);
  } else {
    LFUNode *new_node = malloc(sizeof(LFUNode));
    new_node->key = key;
    new_node->frequency = 1;
    new_node->prev = NULL;
    new_node->next = cache->head;

    if (cache->head)
      cache->head->prev = new_node;
    cache->head = new_node;

    if (!cache->tail)
      cache->tail = new_node;

    cache->hash_table[key] = new_node;
    cache->size++;

    if (cache->size > cache->capacity) {
      int removed_key = cache->tail->key;
      lfu_remove_lfu(cache);
      cache->hash_table[removed_key] = NULL;
    }
  }
}

void lfu_remove(LFUCache *cache, int key) {
  LFUNode *node = cache->hash_table[key];
  if (!node)
    return;

  if (node->prev)
    node->prev->next = node->next;
  if (node->next)
    node->next->prev = node->prev;
  if (cache->head == node)
    cache->head = node->next;
  if (cache->tail == node)
    cache->tail = node->prev;

  free(node);
  cache->hash_table[key] = NULL;
  cache->size--;
}

void lfu_free(LFUCache *cache) {
  LFUNode *current = cache->head;
  while (current) {
    LFUNode *next = current->next;
    free(current);
    current = next;
  }
  free(cache->hash_table);
  free(cache);
}

void lfu_print(LFUCache *cache) {
  LFUNode *current = cache->head;
  printf("LFU Cache: ");
  while (current) {
    printf("[Key: %d, Freq: %d] ", current->key, current->frequency);
    current = current->next;
  }
  printf("\n");
}