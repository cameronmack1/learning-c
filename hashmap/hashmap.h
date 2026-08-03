#ifndef HASHMAP_H
#define HASHMAP_H
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    Node **buckets;
    size_t count;
    size_t capacity;
} Hashmap;

typedef struct Node {
    char* key;
    void* value;
    struct Node* next;
} Node;

Hashmap* init(size_t size);
bool add(Hashmap* map, const char* key, void* value);
bool get(Hashmap* map, const char* key, void** out);
void freeList(Node* list);
void freeMap(Hashmap* map);

#endif