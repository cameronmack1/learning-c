#ifndef GAME_H
#define GAME_H
#include "network.h"
#include <stdbool.h>

typedef struct {
    Network network;
    float x_pos;
    float y_pos;
    float x_vel;
    float y_vel;
    float survival_time;
} Fih;

typedef struct {
    float x_pos;
    float y_pos;
    float x_vel;
    float y_vel;
} Shark;

typedef struct {
    Fih* fih;
    size_t num_fih;
} Game;

#endif