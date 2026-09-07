#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "network.h"

bool init_game(Game** out, size_t fih_count, size_t shark_count) {
    // init game struct
    Game* game = calloc(1, sizeof(Game));
    if (game == NULL) {
        printf("Failed to allocate memory");
        return false;
    }

    // init network for each fih
    game->fih = calloc(fih_count, sizeof(Fih));
    game->num_fih = fih_count;
    if(game->sharks == NULL){
        printf("Failed to allocate memory");
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
    game->sharks = calloc(shark_count, sizeof(Shark));
    game->num_sharks = shark_count;
    if(game->sharks == NULL){
        free(game->fih);
        printf("Failed to allocate memory");
        return false;
    }
}