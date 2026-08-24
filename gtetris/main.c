#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "pieces.h"
#include "render.h"

#define PIXEL_SIZE 40

int main() {
    srand(time(NULL));
    // create window
    InitWindow(10 * PIXEL_SIZE, 20 * PIXEL_SIZE, "no way its tetris");
    SetTargetFPS(60);

    // init game object
    Game* game = NULL;
    if (!init(&game)) {
        printf("Failed to allocate memory");
        return 1;
    }
    // 0left 1right 2cw 3ccw 4hard 5soft
    bool inputs[6] = { false };

    // loop until window closed
    while (!WindowShouldClose()) {
        // get all inputs
        inputs[0] = IsKeyPressed(KEY_LEFT);
        inputs[1] = IsKeyPressed(KEY_RIGHT);
        inputs[2] = IsKeyPressed(KEY_X);
        inputs[3] = IsKeyPressed(KEY_Z);
        inputs[4] = IsKeyPressed(KEY_SPACE);
        inputs[5] = IsKeyDown(KEY_DOWN);

        if(!game->game_over){
            tick(game, inputs);
        }

        BeginDrawing();

        ClearBackground(BLACK);

        render_game(game);

        EndDrawing();
    }
    CloseWindow();
}