// load curses library
#ifdef _WIN32
// use pdcurses if on windows
#include <curses.h>

// use Sleep() on windows
#include <windows.h>
#define sleep_ms(ms) Sleep(ms);
#else
// use ncurses if on linux
#include <ncurses.h>

// use usleep on linux
#include <unistd.h>
#define sleep_ms(ms) usleep(ms * 1000);
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"

int main(){
    srand(time(NULL));
    Game* game = NULL;
    init(&game);
}