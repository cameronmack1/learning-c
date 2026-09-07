#include <raylib.h>
#include <stdbool.h>
#include <stddef.h>

#include "game.h"
#include "render.h"

#define SCALE 6

int convert_units_to_pixels(int value) {
    return value * 6;
}

void render_game(const Game* game) {
    // render fish
    for (int i = 0; i < game->num_fih; i++) {
        DrawCircle(game->fih[i].x_pos * SCALE, game->fih[i].y_pos * SCALE, 1 * SCALE, GREEN);
        DrawCircleLines(game->fih[i].x_pos * SCALE, game->fih[i].y_pos * SCALE, 1 * SCALE, WHITE);
    }

    // render shark
    for(int i = 0; i < game->num_sharks; i++){
        DrawCircle(game->sharks[i].x_pos * SCALE, game->sharks[i].y_pos * SCALE, 5 * SCALE, RED);
        DrawCircleLines(game->sharks[i].x_pos * SCALE, game->sharks[i].y_pos * SCALE, 5 * SCALE, BLACK);
    }
}