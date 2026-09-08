#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "render.h"

#define ROUND_LENGTH 5 //seconds

int max_width = -1;
int max_height = -1;

int main(int argc, char* argv[]) {
    // init random seed
    srand(time(NULL));
    // create window
    InitWindow(1, 1, "no way its fih");
    SetTargetFPS(60);

    // rescale and move window
    SetWindowSize(600, 600);
    SetWindowPosition(100, 100);

    Game* game;
    if (!init_game(&game, 500, 2)) {
        printf("Failed to allocate memory for game\n");
        CloseWindow();
        return 0;
    }

    float timer = 0.0f;

    // loop until window closed
    while (!WindowShouldClose()) {
        tick(game);
        timer += GetFrameTime();

        if(timer > (float)ROUND_LENGTH){
            // end round and start next
            next_round(game);
            timer = 0.0f;
        }

        BeginDrawing();

        // clear bg and render
        ClearBackground(BLUE);

        render_game(game);

        EndDrawing();
    }
    CloseWindow();
}