#ifndef GAME_H
#define GAME_H
#include "network.h"
#include <stdbool.h>

typedef struct {
    Network *network;
    float x_pos;
    float y_pos;
    float x_vel;
    float y_vel;
    float fitness_score;
} Fih;

typedef struct {
    float x_pos;
    float y_pos;
    float x_vel;
    float y_vel;
} Shark;

typedef struct {
    Fih *fih;
    Shark *sharks;
    size_t num_fih;
    size_t num_sharks;
} Game;

bool init_game(Game** out, size_t fih_count, size_t shark_count);
void tick(Game* game);

#endif