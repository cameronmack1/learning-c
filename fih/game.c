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
#define WORLD_WIDTH 100 // 100 units
#define WORLD_HEIGHT 100 // you get the point

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
    if(game->fih == NULL){
        printf("Failed to allocate memory for fih\n");
        return false;
    }
    for (int i = 0; i < fih_count; i++) {
        // 2 weighted layers
        // 6 inputs, 8 hidden neurons, 2 output neurons
        if(!init_network(&game->fih[i].network, 2, 6, 8, 2)){
            // exit if memory alloc failed
            return false;
        }
    }
    // init sharks
    game->sharks = calloc(shark_count, sizeof(Shark));
    game->num_sharks = shark_count;
    if(game->sharks == NULL){
        free(game->fih);
        printf("Failed to allocate memory for sharks\n");
        return false;
    }

    // set to default semirandom positions
    for(int i = 0; i < game->num_fih; i++){
        game->fih[i].x_pos = (WORLD_WIDTH / 2) + rand() % (WORLD_WIDTH / 4) - WORLD_WIDTH / 8;
        game->fih[i].y_pos = (WORLD_HEIGHT / 4) + rand() % (WORLD_HEIGHT / 8) - WORLD_HEIGHT / 16;
    }

    for(int i = 0; i < game->num_sharks; i++){
        game->sharks[i].x_pos = (WORLD_WIDTH / 2) + rand() % (WORLD_WIDTH / 4) - WORLD_WIDTH / 8;
        game->sharks[i].y_pos = (3 * WORLD_HEIGHT / 4) + rand() % (WORLD_HEIGHT / 16) - WORLD_HEIGHT / 32;
    }
    *out = game;
    return true;
}