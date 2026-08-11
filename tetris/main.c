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

int main() {
    srand(time(NULL));
    // init screen
    initscr();
    // stop typed letters from showing on terminal
    noecho();
    // hide terminal cursor
    curs_set(0);
    // enable special keys (arrows, etc)
    keypad(stdscr, TRUE);
    // makes input reading non-blocking so the game runs without waiting for input
    nodelay(stdscr, TRUE);

    Game* game = NULL;
    if (!init(&game)) {
        printf("Failed to allocate memory");
        return 1;
    }

    const int FRAME_DELAY = 1000 / 60;

    // create window
    int max_row, max_col;
    getmaxyx(stdscr, max_row, max_col);

    // start color
    if (start_color() == ERR) {
        endwin();
        printf("start_color() failed\n");
        return 1;
    }

    // exit if no colors
    if (!has_colors()) {
        endwin();
        printf("Terminal does not support colors\n");
        return 1;
    }

    WINDOW* game_window = newwin(22, 22, max_row - 22, max_col / 2 - 11);

    // colors
    init_pair(PIECE_I, COLOR_CYAN, COLOR_CYAN);
    init_pair(PIECE_T, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(PIECE_O, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(PIECE_S, COLOR_GREEN, COLOR_GREEN);
    init_pair(PIECE_Z, COLOR_RED, COLOR_RED);
    init_pair(PIECE_J, COLOR_BLUE, COLOR_BLUE);
    init_pair(PIECE_L, COLOR_YELLOW, COLOR_YELLOW);

    // main game loop
    while (1) {
        werase(game_window);

        bool input[6] = { false };
        int ch;
        while ((ch = getch()) != ERR) {
            switch (ch) {
            // quit
            case 'Q':
            case 'q': {
                free(game);

                endwin();
                return (0);
            }

            // move left
            case KEY_LEFT: {
                input[0] = true;
                break;
            }

            // move right
            case KEY_RIGHT: {
                input[1] = true;
                break;
            }

            // rotate cw
            case 'X':
            case 'x': {
                input[2] = true;
                break;
            }

            // rotate ccw
            case 'Z':
            case 'z': {
                input[3] = true;
                break;
            }

            // hard drop
            case ' ': {
                input[4] = true;
                break;
            }

            // soft drop
            case KEY_DOWN: {
                input[5] = true;
                break;
            }
            }
        }
        // tick game
        tick(game, input);

        // draw box
        box(game_window, 0, 0);

        // draw current piece
        wattron(game_window, COLOR_PAIR(game->current.type));
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (game->current.shape[i][j]) {
                    mvwprintw(game_window, game->current.y + i, (game->current.x + j) * 2, "##");
                }
            }
        }
        wattroff(game_window, COLOR_PAIR(game->current.type));
        
        // update game window
        wnoutrefresh(game_window);

        // update screen
        doupdate();
        // sleep until next frame
        sleep_ms(FRAME_DELAY);
    }
}