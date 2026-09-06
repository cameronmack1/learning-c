#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"

int max_width = -1;
int max_height = -1;

int main() {
    // init random seed
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

    // loop until window closed
    while (!WindowShouldClose()) {
    }
    CloseWindow();
}