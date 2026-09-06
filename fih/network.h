#ifndef NETWORK_H
#define NETWORK_H
#include <stddef.h>

typedef struct {
    int num_weights;
    int num_neurons;

    float *weights;
    float *bias;
    float *output;
} Layer;

typedef struct {
    Layer *layers;
    size_t num_layers;
} Network;

bool init(Network** out, size_t num_layers, ...);

#endif