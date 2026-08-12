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

#include "pieces.h"
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
    WINDOW* score_window = newwin(7, 10, max_row - 22, max_col / 2 - 23);
    WINDOW* next_window = newwin(7, 10, max_row - 15, max_col / 2 - 23);

    // colors
    init_pair(PIECE_I, COLOR_CYAN, COLOR_CYAN);
    init_pair(PIECE_T, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(PIECE_O, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(PIECE_S, COLOR_GREEN, COLOR_GREEN);
    init_pair(PIECE_Z, COLOR_RED, COLOR_RED);
    init_pair(PIECE_J, COLOR_BLUE, COLOR_BLUE);
    init_pair(PIECE_L, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(8, COLOR_WHITE, COLOR_WHITE);

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
        if(!game->game_over){
            tick(game, input);
        }

        // draw box
        box(game_window, 0, 0);

        // draw current piece and ghost
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (game->current.shape[i][j]) {
                    // draw ghost
                    wattron(game_window, COLOR_PAIR(8));
                    mvwprintw(game_window, game->current.y + i + 1 + game->ghost_dy, (game->current.x + j) * 2 + 1, "##");
                    wattroff(game_window, COLOR_PAIR(8));

                    if ((game->current.y + i) < 0)
                        continue;
                    // draw piece
                    wattron(game_window, COLOR_PAIR(game->current.type));
                    mvwprintw(game_window, game->current.y + i + 1, (game->current.x + j) * 2 + 1, "##");
                    wattroff(game_window, COLOR_PAIR(game->current.type));
                }
            }
        }

        // draw current map
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 10; j++) {
                if (game->board[i][j]) {
                    wattron(game_window, COLOR_PAIR(game->board[i][j]));
                    mvwprintw(game_window, i + 1, j * 2 + 1, "##");
                    wattroff(game_window, COLOR_PAIR(game->board[i][j]));
                }
            }
        }

        // update game window
        wnoutrefresh(game_window);

        box(score_window, 0, 0);

        // print score
        int len = snprintf(NULL, 0, "%d", game->score);
        mvwprintw(score_window, 1, 2, "SCORE");
        mvwprintw(score_window, 2, (10 - len) / 2, "%d", game->score);

        // print lines
        len = snprintf(NULL, 0, "%d", game->lines);
        mvwprintw(score_window, 4, 2, "LINES");
        mvwprintw(score_window, 5, (10 - len) / 2, "%d", game->lines);

        // update score window
        wnoutrefresh(score_window);

        // erase window
        werase(next_window);

        box(next_window, 0, 0);
        mvwprintw(next_window, 1, 3, "NEXT");
        wattron(next_window, COLOR_PAIR(game->next.type));
        // draw next piece
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (game->next.shape[i][j] == 0)
                    continue;
                //print 1 higher if it is an O piece, print 1 space to the left if it is an O or an I piece
                mvwprintw(next_window, 3 + i - (game->next.type == PIECE_O), 1 + (2 * j) + 4 - piece_sizes[game->next.type], "##");
            }
        }
        wattroff(next_window, COLOR_PAIR(game->next.type));

        // update score window
        wnoutrefresh(next_window);

        // update screen
        doupdate();
        // sleep until next frame
        sleep_ms(FRAME_DELAY);
    }
}