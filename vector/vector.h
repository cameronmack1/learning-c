#ifndef VECTOR_H
#define VECTOR_H
#include <stddef.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} Vector;

Vector* init(size_t size);
bool push(Vector* vec, int value);
bool get(Vector* vec, size_t index, int* out);
bool shrink(Vector* vec);
Vector* clear(Vector* vec);
void freeVector(Vector* vec);

#endif