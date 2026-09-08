#ifndef NETWORK_H
#define NETWORK_H
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    int num_weights;
    int num_neurons;
    int num_inputs;

    float *weights;
    float *bias;
    float *output;
} Layer;

typedef struct {
    Layer *layers;
    size_t num_layers;
} Network;

bool init_network(Network** out, size_t num_layers, ...);
void forward_propagate(Network *net, const float *inputs);
void mutate(Network *net, float mutation_rate, float mutation_strength);
void copy_network(Network *in, Network *out);

#endif