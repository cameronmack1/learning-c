#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "pieces.h"
#include "render.h"

int max_width = -1;
int max_height = -1;

int get_high_score() {
    // create and open file stream
    FILE* fptr;
    fptr = fopen("highscore.txt", "r");

    // if file does not exist, or it was unable to be opened for whatever reason, return 0
    if (fptr == NULL)
        return 0;

    int number;

    // attempt to read a number from it, and return it if successful
    if (fscanf(fptr, "%d", &number) == 1) {
        fclose(fptr);
        return number;
    }

    fclose(fptr);
    return 0;
}

bool set_high_score(int score) {
    // create and open file stream
    FILE* fptr;
    fptr = fopen("highscore.txt", "w");

    // if file was unable to be opened for whatever reason, return false
    if (fptr == NULL) {
        printf("Failed to save high score");
        return false;
    }

    // put the new score in the highscore.txt file
    fprintf(fptr, "%d", score);

    return true;
}

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

    // das settings
    int das_delay = 16;
    int das_repeat_delay = 6;

    int das_left_timer = 0;
    int das_right_timer = 0;

    // high score
    int high_score = 0;
    // prevent loading high score from file multiple times
    bool game_over_loaded = false;

    // loop until window closed
    while (!WindowShouldClose()) {
        // left/right DAS system
        bool left = IsKeyDown(KEY_LEFT);
        bool right = IsKeyDown(KEY_RIGHT);

        inputs[0] = false;
        if (left) {
            if (das_left_timer == 0) {
                inputs[0] = true;
            } else if (das_left_timer >= das_delay) {
                inputs[0] = true;
                das_left_timer -= das_repeat_delay;
            }
            das_left_timer++;
        } else {
            das_left_timer = 0;
        }

        inputs[1] = false;
        if (right) {
            if (das_right_timer == 0) {
                inputs[1] = true;
            } else if (das_right_timer >= das_delay) {
                inputs[1] = true;
                das_right_timer -= das_repeat_delay;
            }
            das_right_timer++;
        } else {
            das_right_timer = 0;
        }

        // handle inputs
        inputs[2] = IsKeyPressed(KEY_X);
        inputs[3] = IsKeyPressed(KEY_Z);
        inputs[4] = IsKeyPressed(KEY_SPACE);
        inputs[5] = IsKeyDown(KEY_DOWN);

        // update game state
        if (!game->game_over) {
            tick(game, inputs);
        } else if (!game_over_loaded) {
            game_over_loaded = true;

            // get high score
            high_score = get_high_score();

            // set high score if current score is > high score
            if (game->score > high_score) {
                set_high_score(game->score);
            }
        }

        // render
        BeginDrawing();

        // clear bg and render
        ClearBackground(BLACK);
        render_game(game, high_score);

        EndDrawing();

        if (game->game_over && IsKeyPressed(KEY_ENTER)) {
            // we dont want a memory leak do we
            free(game);
            if (!init(&game)) {
                printf("Failed to allocate memory");
                return 1;
            }
            game_over_loaded = false;
        }
    }
    CloseWindow();
}