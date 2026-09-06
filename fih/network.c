#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "network.h"

bool create_add_layer(Network* network, int in_size, int layer_size, int num_layer) {
    Layer* layer = &network->layers[num_layer];
    // create values
    layer->num_neurons = layer_size;
    layer->num_inputs = in_size;
    layer->num_weights = in_size * layer_size;

    layer->bias = calloc(layer_size, sizeof(float));
    layer->weights = calloc(layer->num_weights, sizeof(float));
    layer->output = calloc(layer_size, sizeof(float));

    if (layer->bias == NULL || layer->weights == NULL || layer->output == NULL) {
        printf("Failed to allocate memory");
        return false;
    }
    return true;
}

bool init(Network** out, size_t num_layers, ...) {
    if (num_layers < 2)
        return false;

    Network* network;
    network = calloc(1, sizeof(Network));
    if (network == NULL) {
        printf("Failed to allocate memory");
        return false;
    }

    network->layers = calloc(num_layers, sizeof(Layer));
    if (network->layers == NULL) {
        printf("Failed to allocate memory");
        free(network);
        return false;
    }

    network->num_layers = num_layers;

    // create variadic function thing
    va_list args;

    va_start(args, num_layers);

    int inputs = va_arg(args, int);
    if (!create_add_layer(network, inputs, va_arg(args, int), 0)) {
        free(network->layers[0].bias);
        free(network->layers[0].weights);
        free(network->layers[0].output);
        free(network->layers);
        free(network);
        va_end(args);
        return false;
    }

    // loop thru every variadic input
    for (int i = 1; i < num_layers; i++) {
        if (!create_add_layer(network, network->layers[i - 1].num_neurons, va_arg(args, int), i)) {
            printf("Failed to allocate memory");
            for (int j = 0; j <= i; j++) {
                free(network->layers[j].bias);
                free(network->layers[j].weights);
                free(network->layers[j].output);
            }
            free(network->layers);
            free(network);
            va_end(args);
            return false;
        }
    }
    // make sure to end it
    va_end(args);

    *out = network;
    return true;
}

void calculate_layer(Layer* layer, const float* inputs, float (*activation)(float)) {
    // loop over every neuron
    for (int i = 0; i < layer->num_neurons; i++) {
        // unflatten array
        int weights_index = i * layer->num_inputs;

        // add bias
        float sum = 0;
        sum += layer->bias[i];

        // loop over every input
        for (int j = 0; j < layer->num_inputs; j++) {
            // output = b + w1*i1 + w2i2....
            sum += layer->weights[weights_index + j] * inputs[j];
        }
        // activation function
        layer->output[i] = activation(sum);
    }
}

void forward_propagate(Network* net, const float* inputs) {
    for (int i = 0; i < net->num_layers; i++) {
        // sets the inputs to be the last layers outputs, or the passed in inputs depending on what layer it is on
        const float* cur_inputs = (i == 0) ? inputs : net->layers[i].output;
    }
}