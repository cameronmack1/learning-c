#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "network.h"

// fish details
#define FIH_MAX_VEL 20 // 20 u/s
#define FIH_MAX_ACCEL 40 // 40 u/s^2
#define FIH_RADIUS 1 // 1 unit
// shark details
#define SHARK_MAX_VEL 30 // 10 u/s^2
#define SHARK_MAX_ACCEL 20 // 10 u/s
#define SHARK_RADIUS 5 // 5 units
// world details
#define WORLD_WIDTH 150 // 1000 units
#define WORLD_HEIGHT 150 // you get the point

bool init_game(Game** out, size_t fih_count, size_t shark_count) {
    // init game struct
    Game* game = calloc(1, sizeof(Game));
    if (game == NULL) {
        printf("Failed to allocate memory for game\n");
        return false;
    }

    // init memory for everything
    // init network for each fih
    game->fih = calloc(fih_count, sizeof(Fih));
    game->num_fih = fih_count;
    if (game->fih == NULL) {
        printf("Failed to allocate memory for fih\n");
        return false;
    }
    for (int i = 0; i < fih_count; i++) {
        // 2 weighted layers
        // 10 inputs, 12 hidden neurons, 2 output neurons
        if (!init_network(&game->fih[i].network, 2, 10, 12, 2)) {
            // exit if memory alloc failed
            return false;
        }
    }
    // init sharks
    game->sharks = calloc(shark_count, sizeof(Shark));
    game->num_sharks = shark_count;
    if (game->sharks == NULL) {
        free(game->fih);
        printf("Failed to allocate memory for sharks\n");
        return false;
    }

    // set to default semirandom positions
    for (int i = 0; i < game->num_fih; i++) {
        game->fih[i].x_pos = (WORLD_WIDTH / 2) + rand() % (WORLD_WIDTH / 4) - WORLD_WIDTH / 8;
        game->fih[i].y_pos = (WORLD_HEIGHT / 4) + rand() % (WORLD_HEIGHT / 8) - WORLD_HEIGHT / 16;
    }

    for (int i = 0; i < game->num_sharks; i++) {
        game->sharks[i].x_pos = (WORLD_WIDTH / 2) + rand() % (WORLD_WIDTH / 4) - WORLD_WIDTH / 8;
        game->sharks[i].y_pos = (3 * WORLD_HEIGHT / 4) + rand() % (WORLD_HEIGHT / 16) - WORLD_HEIGHT / 32;
    }
    *out = game;
    return true;
}

float magnitude(float val1, float val2) {
    return sqrtf(val1 * val1 + val2 * val2);
}

void tick(Game* game) {
    // reset nearest distance to each fish for all sharks
    for (int i = 0; i < game->num_sharks; i++) {
        game->sharks[i].nearest_dist = __FLT_MAX__;
        game->sharks[i].nearest_fih = -1;
    }
    // move fish
    // fish inputs are
    // vx, vy, sdx, sdy, svx, svy, ldx, rdx, udy, ddy
    for (int i = 0; i < game->num_fih; i++) {
        if (game->fih[i].is_dead)
            continue;
        float inputs[10] = { 0.0f };
        inputs[0] = game->fih[i].x_vel / FIH_MAX_VEL;
        inputs[1] = game->fih[i].y_vel / FIH_MAX_VEL;

        // find index of nearest shark
        int shark_index = 0;
        // use euclidean distance
        float best_euclid = __FLT_MAX__;
        for (int j = 0; j < game->num_sharks; j++) {
            // a^2 + b^2
            float dx = game->sharks[j].x_pos - game->fih[i].x_pos;
            float dy = game->sharks[j].y_pos - game->fih[i].y_pos;
            float dist = dx * dx + dy * dy;
            if (dist < best_euclid) {
                best_euclid = dist;
                shark_index = j;
            }
            if (dist < game->sharks[j].nearest_dist) {
                game->sharks[j].nearest_dist = dist;
                game->sharks[j].nearest_fih = i;
            }
        }

        // dx and dy
        inputs[2] = (game->fih[i].x_pos - game->sharks[shark_index].x_pos) / WORLD_WIDTH;
        inputs[3] = (game->fih[i].y_pos - game->sharks[shark_index].y_pos) / WORLD_WIDTH;

        // svx and svy
        inputs[4] = game->sharks[shark_index].x_vel / SHARK_MAX_VEL;
        inputs[5] = game->sharks[shark_index].y_vel / SHARK_MAX_VEL;

        inputs[6] = (game->fih[i].x_pos - FIH_RADIUS) / WORLD_WIDTH;
        inputs[7] = (WORLD_WIDTH - game->fih[i].x_pos - FIH_RADIUS) / WORLD_WIDTH;
        inputs[8] = (game->fih[i].y_pos - FIH_RADIUS) / WORLD_HEIGHT;
        inputs[9] = (WORLD_HEIGHT - game->fih[i].y_pos - FIH_RADIUS) / WORLD_HEIGHT;

        // propagate
        forward_propagate(game->fih[i].network, inputs);

        // handle output
        // outputs are
        // x_accel, y_accel
        float x_accel = game->fih[i].network->layers[game->fih[i].network->num_layers - 1].output[0];
        float y_accel = game->fih[i].network->layers[game->fih[i].network->num_layers - 1].output[1];

        float mag = magnitude(x_accel, y_accel);
        // normalize acceleration
        if (mag > 1.0f) {
            x_accel /= mag;
            y_accel /= mag;
        }
        x_accel *= (float)FIH_MAX_ACCEL;
        y_accel *= (float)FIH_MAX_ACCEL;
        game->fih[i].x_vel += x_accel / 60.0f;
        game->fih[i].y_vel += y_accel / 60.0f;
        // normalize velocity
        mag = magnitude(game->fih[i].x_vel, game->fih[i].y_vel);
        if (mag > (float)FIH_MAX_VEL) {
            game->fih[i].x_vel /= (mag / (float)FIH_MAX_VEL);
            game->fih[i].y_vel /= (mag / (float)FIH_MAX_VEL);
        }
        game->fih[i].x_pos += game->fih[i].x_vel / 60.0f;
        game->fih[i].y_pos += game->fih[i].y_vel / 60.0f;

        // make sure fish stay on screen
        if (game->fih[i].x_pos < FIH_RADIUS) {
            game->fih[i].x_pos = FIH_RADIUS;
            game->fih[i].x_vel = 0;
        } else if (game->fih[i].x_pos > WORLD_WIDTH - FIH_RADIUS) {
            game->fih[i].x_pos = WORLD_WIDTH - FIH_RADIUS;
            game->fih[i].x_vel = 0;
        }

        if (game->fih[i].y_pos < FIH_RADIUS) {
            game->fih[i].y_pos = FIH_RADIUS;
            game->fih[i].y_vel = 0;
        } else if (game->fih[i].y_pos > WORLD_HEIGHT - FIH_RADIUS) {
            game->fih[i].y_pos = WORLD_HEIGHT - FIH_RADIUS;
            game->fih[i].y_vel = 0;
        }
    }
    // move shark
    // loop over every shark
    // we found the nearest fish to each shark at the start during the fish loop
    for (int i = 0; i < game->num_sharks; i++) {
        float x_accel = game->fih[game->sharks[i].nearest_fih].x_pos - game->sharks[i].x_pos;
        float y_accel = game->fih[game->sharks[i].nearest_fih].y_pos - game->sharks[i].y_pos;
        // normalize and add acceleration to velocity
        float mag = magnitude(x_accel, y_accel);
        x_accel /= mag / (float)SHARK_MAX_ACCEL;
        y_accel /= mag / (float)SHARK_MAX_ACCEL;
        game->sharks[i].x_vel += x_accel / 60.0f;
        game->sharks[i].y_vel += y_accel / 60.0f;
        // normalize and add velocity to pos
        mag = magnitude(game->sharks[i].x_vel, game->sharks[i].y_vel);
        if (mag > SHARK_MAX_VEL) {
            game->sharks[i].x_vel /= mag / (float)SHARK_MAX_VEL;
            game->sharks[i].y_vel /= mag / (float)SHARK_MAX_VEL;
        }
        game->sharks[i].x_pos += game->sharks[i].x_vel / 60.0f;
        game->sharks[i].y_pos += game->sharks[i].y_vel / 60.0f;

        // prevent going off screen
        if (game->sharks[i].x_pos < SHARK_RADIUS) {
            game->sharks[i].x_pos = SHARK_RADIUS;
            game->sharks[i].x_vel = 0;
        } else if (game->sharks[i].x_pos > WORLD_WIDTH - SHARK_RADIUS) {
            game->sharks[i].x_pos = WORLD_WIDTH - SHARK_RADIUS;
            game->sharks[i].x_vel = 0;
        }

        if (game->sharks[i].y_pos < SHARK_RADIUS) {
            game->sharks[i].y_pos = SHARK_RADIUS;
            game->sharks[i].y_vel = 0;
        } else if (game->sharks[i].y_pos > WORLD_HEIGHT - SHARK_RADIUS) {
            game->sharks[i].y_pos = WORLD_HEIGHT - SHARK_RADIUS;
            game->sharks[i].y_vel = 0;
        }
    }

    // loop thru every fish, check if dead, add fitness
    for (int i = 0; i < game->num_fih; i++) {
        float min_dist = __FLT_MAX__;
        // loop thru every shark to check if dead and find distance to nearest shark
        for (int j = 0; j < game->num_sharks; j++) {
            float dx = game->sharks[j].x_pos - game->fih[i].x_pos;
            float dy = game->sharks[j].y_pos - game->fih[i].y_pos;

            float dist = sqrt(dx * dx + dy * dy);
            if (dist < min_dist) {
                min_dist = dist;
                if (min_dist < FIH_RADIUS + SHARK_RADIUS) {
                    game->fih[i].is_dead = true;
                }
            }
        }
        if (!game->fih[i].is_dead) {
            game->fih[i].fitness_score += 1.0f + (1 / min_dist);
        }
    }
}

// take the 5 best networks, copy them to cover the entire thing, and mutate
void next_round(Game* game) {
    // lowest to highest
    int best_fih[5] = { -1 };
    float best_scores[5] = { -1.0f };

    for (int i = 0; i < game->num_fih; i++) {
        // if score is lower than minimum, ignore
        if (game->fih[i].fitness_score < best_scores[0])
            continue;

        int j = 1;
        // shift elements until we reach the one we want
        while (j < 5 && best_scores[j] < game->fih[i].fitness_score) {
            best_scores[j - 1] = best_scores[j];
            best_fih[j - 1] = best_fih[j];
            j++;
        }

        // insert new fish
        best_scores[j] = game->fih[i].fitness_score;
        best_fih[j] = i;
    }

    for (int i = 0; i < game->num_fih; i++) {
        // make sure we dont modify any of the parents
        bool is_parent = false;
        for (int j = 0; j < 5; j++) {
            if (i == j) {
                is_parent = true;
                break;
            }
        }
        if (is_parent) {
            continue;
        }
    }
}