#include "lru.h"

LRUCache *lru_create(size_t capacity) {
  LRUCache *cache = malloc(sizeof(LRUCache));
  cache->capacity = capacity;
  cache->size = 0;
  cache->head = NULL;
  cache->tail = NULL;
  cache->hash_table = calloc(capacity, sizeof(LRUNode *));
  return cache;
}

void lru_move_to_front(LRUCache *cache, LRUNode *node) {
  if (cache->head == node)
    return;

  if (node->prev)
    node->prev->next = node->next;
  if (node->next)
    node->next->prev = node->prev;
  if (cache->tail == node)
    cache->tail = node->prev;

  node->next = cache->head;
  node->prev = NULL;

  if (cache->head)
    cache->head->prev = node;
  cache->head = node;

  if (!cache->tail)
    cache->tail = node;
}

void lru_remove_lru(LRUCache *cache) {
  if (!cache->tail)
    return;

  LRUNode *lru = cache->tail;
  if (lru->prev)
    lru->prev->next = NULL;
  else
    cache->head = NULL;

  cache->tail = lru->prev;
  free(lru);
  cache->size--;
}

void lru_access(LRUCache *cache, int key) {
  if (cache->hash_table[key]) {
    lru_move_to_front(cache, cache->hash_table[key]);
  } else {
    LRUNode *new_node = malloc(sizeof(LRUNode));
    new_node->key = key;
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
      lru_remove_lru(cache);
      cache->hash_table[removed_key] = NULL;
    }
  }
}

void lru_remove(LRUCache *cache, int key) {
  LRUNode *node = cache->hash_table[key];
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

void lru_free(LRUCache *cache) {
  LRUNode *current = cache->head;
  while (current) {
    LRUNode *next = current->next;
    free(current);
    current = next;
  }
  free(cache->hash_table);
  free(cache);
}

void lru_print(LRUCache *cache) {
  LRUNode *current = cache->head;
  printf("LRU Cache: ");
  while (current) {
    printf("%d ", current->key);
    current = current->next;
  }
  printf("\n");
}