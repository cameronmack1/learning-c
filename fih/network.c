#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "network.h"

bool create_add_layer(Network* network, int in_size, int layer_size, int num_layer, bool randomize) {
    Layer* layer = &network->layers[num_layer];
    // create values
    layer->num_neurons = layer_size;
    layer->num_inputs = in_size;
    layer->num_weights = in_size * layer_size;

    layer->bias = calloc(layer_size, sizeof(float));
    layer->weights = calloc(layer->num_weights, sizeof(float));
    layer->output = calloc(layer_size, sizeof(float));

    if (layer->bias == NULL || layer->weights == NULL || layer->output == NULL) {
        printf("Failed to allocate memory for layer bias/weights/output\n");
        return false;
    }

    // uniform random from -1 to 1
    if (randomize) {
        for (int i = 0; i < layer->num_weights; i++) {
            layer->weights[i] = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
        }
        for (int i = 0; i < layer->num_neurons; i++) {
            layer->bias[i] = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
        }
    }
    return true;
}

bool init_network(Network** out, size_t num_layers, ...) {
    if (num_layers < 2)
        return false;

    Network* network;
    network = calloc(1, sizeof(Network));
    if (network == NULL) {
        printf("Failed to allocate memory for network\n");
        return false;
    }

    network->layers = calloc(num_layers, sizeof(Layer));
    if (network->layers == NULL) {
        printf("Failed to allocate memory for layer\n");
        free(network);
        return false;
    }

    network->num_layers = num_layers;

    // create variadic function thing
    va_list args;

    va_start(args, num_layers);

    int inputs = va_arg(args, int);
    int layer_0 = va_arg(args, int);
    if (!create_add_layer(network, inputs, layer_0, 0, true)) {
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
        if (!create_add_layer(network, network->layers[i - 1].num_neurons, va_arg(args, int), i, true)) {
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

// tanh activaiton function
float tanh_activation(float value) {
    return tanh(value);
}

// ReLu activation functino
float relu_activation(float value) {
    return value > 0 ? value : 0;
}

// box muller transform, polar form
float box_muller_transform() {
    static float z2;
    static bool z2_saved = false;

    if (z2_saved) {
        z2_saved = false;
        return z2;
    }

    float u, v, w;

    do {
        // set u and v to 2 random numbers between -1 and 0
        u = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
        v = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;

        // w = u^2 + v^2
        // if w = 0 or w >= 1 or w <= -1, then pick new numbers
        w = u * u + v * v;

        // loop until valid numbers found
    } while (w == 0.0f || w > 1.0f);

    w = sqrtf((-2.0f * logf(w)) / w);

    // calculate final numbers
    float z1 = u * w;
    z2 = v * w;

    // show that z2 is saved for the next call
    z2_saved = true;

    return z1;
}

void forward_propagate(Network* net, const float* inputs) {
    // loop thru every layer
    for (int i = 0; i < net->num_layers; i++) {
        // sets the inputs to be the last layers outputs, or the passed in inputs depending on what layer it is on
        const float* cur_inputs = (i == 0) ? inputs : net->layers[i - 1].output;

        // use ReLu for hidden layers, tanh for output
        calculate_layer(&net->layers[i], cur_inputs, (i == net->num_layers - 1) ? tanh_activation : relu_activation);
    }
}

// mutate network
void mutate(Network* net, float mutation_rate, float mutation_strength) {
    // loop thru every layer
    for (int i = 0; i < net->num_layers; i++) {
        // loop thru every weight
        for (int j = 0; j < net->layers[i].num_weights; j++) {
            // random chance to mutate
            if ((float)rand() / RAND_MAX < mutation_rate) {
                // use box muller transform for a random gaussian distributed number, multiply by mutation strength
                net->layers[i].weights[j] += box_muller_transform() * mutation_strength;
            }
        }
        // do it again for all the biases
        for (int j = 0; j < net->layers[i].num_neurons; j++) {
            if ((float)rand() / RAND_MAX < mutation_rate) {
                net->layers[i].bias[j] += box_muller_transform() * mutation_strength;
            }
        }
    }
}

// this will NOT work if layer sizes are not even
void copy_network(Network *in, Network *out){   
    // loop over each layer
    for(int i = 0; i < in->num_layers; i++){
        // copy all weights
        for(int j = 0; j < in->layers[i].num_weights; j++){
            out->layers[i].weights[j] = in->layers[i].weights[j];
        }
        // copy all bias
        for(int j = 0; j < in->layers[i].num_neurons; j++){
            out->layers[i].bias[j] = in->layers[i].bias[j];
        }
    }
}