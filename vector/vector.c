#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

//create vector
Vector* init(size_t size){
    Vector* vec = malloc(sizeof(Vector));
    if(vec == NULL){
        printf("Failed to allocate memory\n");
        return NULL;
    }
    vec->data = malloc(sizeof(int) * size);
    if(vec->data == NULL){
        printf("Failed to allocate memory\n");
        //prevent memory leak
        free(vec);
        return NULL;
    }
    vec->capacity = size;
    vec->size = 0;
    return vec;
}

//add item
bool push(Vector* vec, int value){
    if(vec == NULL) return false;
    //allocate more memory if size is equal to capacity
    if(vec->size == vec->capacity){
        //double capacity, or set to 4 if it is 0
        size_t newCapacity = vec->capacity == 0 ? 4 : vec->capacity * 2;
        int* newPtr = realloc(vec->data, sizeof(int) * newCapacity);
        //return false if realloc fails
        if(newPtr == NULL) return false;
        vec->data = newPtr;
        vec->capacity = newCapacity;
    }
    //add new value and return true
    vec->data[vec->size++] = value;
    return true;
}

//get value and put it in a pointer
bool get(Vector* vec, size_t index, int* out){
    if(vec == NULL || out == NULL) return false;
    if(index >= vec->size) return false;
    *out = vec->data[index];
    return true;
}

//shrink it so no extra memory is being taken up
bool shrink(Vector* vec){
    if(vec == NULL) return false;
    //shrink to 1 byte if size is 0
    if(vec->size == 0) {
        int* newPtr = realloc(vec->data, sizeof(int) * 1);
        if(newPtr == NULL) return false;
        vec->data = newPtr;
        vec->capacity = 1;
        return true;
    }
    int* newPtr = realloc(vec->data, sizeof(int) * vec->size);
    //return false if realloc failed
    if(newPtr == NULL) return false;
    vec->data = newPtr;
    vec->capacity = vec->size;
    return true;
}

//clear a vector
Vector* clear(Vector* vec){
    if(vec == NULL) return NULL;
    //just set size to 0
    vec->size = 0;
    return vec;
}

//free the memory
void freeVector(Vector* vec){
    if(vec == NULL) return;
    free(vec->data);
    free(vec);
}