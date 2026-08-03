#include "hashmap.h"
#include <stdbool.h>
#include <stddef.h>

// creation functions
// init the hashmap with a size
Hashmap* init(size_t size)
{
    Hashmap* map = malloc(sizeof(Hashmap));
    if (map == NULL) {
        printf("Failed to allocate memory");
        return NULL;
    }
    map->buckets = calloc(size, sizeof(Node*));
    if (map->buckets == NULL) {
        free(map);
        printf("Failed to allocate memory");
        return NULL;
    }
    map->count = 0;
    map->capacity = size;
    return map;
}

// calculates the hash code of a string key (djb2)
unsigned long hashCode(const char* key)
{
    unsigned long hash = 5381;
    const unsigned char* ptr = key;
    int c;

    // loop thru string
    while (c = *ptr++) {
        hash = ((hash << 5) + hash) + c; // hash*33+char
    }
    return hash;
}

// helper for creating a node when adding one
Node* createNode(const char* key, void* value)
{
    Node* node = malloc(sizeof(Node));
    // return error if malloc failed
    if (node == NULL) {
        printf("Failed to allocate memory");
        return NULL;
    }

    node->key = strdup(key); // copy the string
    node->value = value;
    node->next = NULL;
    return node;
}

// add new value to map, using a string key and any value
bool add(Hashmap* map, const char* key, void* value)
{
    if (map == NULL)
        return false;
    // calculate hash code
    unsigned long code = hashCode(key) % map->capacity;
    // add to hashmap
    if (map->buckets[code] == NULL) {
        // if it is a new bucket
        // create node and return if its successful
        map->buckets[code] = createNode(key, value);
        map->count++;
        // resize if we are >75% full
        if (map->count * 4 > map->capacity * 3)
            resize(map);
        return map->buckets[code] != NULL;
    }
    else {
        // handle collision
        Node* prev = map->buckets[code];
        Node* current = prev;
        // loop through list to look for duplicate value to update
        while (current != NULL) {
            if (strcmp(current->key, key) == 0) {
                current->value = value;
                return true;
            }
            prev = current;
            current = current->next;
        }
        // create node and return if its successful
        prev->next = createNode(key, value);
        map->count++;
        // resize if we are >75% full
        if (map->count * 4 > map->capacity * 3)
            resize(map);
        return prev->next != NULL;
    }
}

// remove key value pair
bool remove(Hashmap* map, const char* key)
{
    if (map == NULL || key == NULL)
        return false;
    // get hash code and bucket
    unsigned long code = hashCode(key) % map->capacity;
    Node* list = map->buckets[code];
    if (list == NULL)
        return false;
    // if the first element matches
    if (strcmp(list->key, key) == 0) {
        map->buckets[code] = list->next;
        free(list->key);
        free(list);
        map->count--;
        return true;
    }
    // loop through list
    Node* current = list;
    while (current->next != NULL) {
        // if key matches
        if (strcmp(current->next->key, key) == 0) {
            // link nodes, free memory and return
            Node* toFree = current->next;
            current->next = toFree->next;
            free(toFree->key);
            free(toFree);
            map->count--;
            return true;
        }
        // increment
        current = current->next;
    }
    return false;
}

// utility
// gets a value from the hashmap and returns it to the out pointer, returns true if success
bool get(Hashmap* map, const char* key, void** out)
{
    if (map == NULL || key == NULL)
        return false;
    // get hash code and bucket
    unsigned long code = hashCode(key) % map->capacity;
    Node* list = map->buckets[code];
    // loop thru bucket until we find it
    while (list != NULL) {
        // check if key matches and pass it to the out pointer if it does
        if (strcmp(list->key, key) == 0) {
            if (out != NULL)
                *out = list->value;
            return true;
        }
        list = list->next;
    }
    // return a fail if it was not found
    return false;
}

// memory
// free a list
void freeList(Node* list)
{
    if (list == NULL)
        return;
    Node* current = list;
    Node* next;
    // loop through and free the list
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

// free the hashmap
void freeMap(Hashmap* map)
{
    if (map == NULL)
        return;
    for (int i = 0; i < map->capacity; i++) {
        freeList(map->buckets[i]);
    }
    free(map);
}

bool resize(Hashmap* map)
{
    // create variables for both buckets
    Node** oldBuckets = map->buckets;
    Node** newBuckets = calloc(map->capacity * 2, sizeof(Node*));

    // exit if memory failed to allocate
    if (newBuckets == NULL)
        return false;

    // variables for both capacities
    int oldCapacity = map->capacity;
    int newCapacity = oldCapacity * 2;

    // set maps new values
    map->buckets = newBuckets;
    map->capacity = newCapacity;

    // loop thru every bucket
    for (int i = 0; i < oldCapacity; i++) {
        Node* current = oldBuckets[i];
        // loop thru every node in list
        while (current != NULL) {
            Node* next = current->next;

            int newCode = hashCode(current->key) % newCapacity;
            // add current node to the start of the bucket we want it to go to
            current->next = newBuckets[newCode];
            newBuckets[newCode] = current;

            // increment
            current = next;
        }
    }

    free(oldBuckets);
    return true;
}