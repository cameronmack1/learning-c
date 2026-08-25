#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "pieces.h"
#include "render.h"

int max_width =-1;
int max_height = -1;

int main() {
    srand(time(NULL));
    // create window
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(1, 1, "no way its tetris");
    SetTargetFPS(60);

    int monitor = GetCurrentMonitor();

    // get max width and height
    int max_width = GetMonitorWidth(monitor);
    int max_height = GetMonitorHeight(monitor);

    // rescale and move window
    SetWindowSize(max_width, max_height);
    SetWindowPosition(0, 0);

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
        // handle inputs
        inputs[0] = IsKeyPressed(KEY_LEFT);
        inputs[1] = IsKeyPressed(KEY_RIGHT);
        inputs[2] = IsKeyPressed(KEY_X);
        inputs[3] = IsKeyPressed(KEY_Z);
        inputs[4] = IsKeyPressed(KEY_SPACE);
        inputs[5] = IsKeyDown(KEY_DOWN);

        // update game state
        if (!game->game_over) {
            tick(game, inputs);
        }

        // render
        BeginDrawing();

        // clear bg and render
        ClearBackground(BLACK);
        render_game(game);

        EndDrawing();
    }
    CloseWindow();
}