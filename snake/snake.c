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

typedef enum {
    UP,
    LEFT,
    DOWN,
    RIGHT
} Direction;

typedef enum {
    DEFAULT,
    APPLE,
    SNAKE_HEAD,
    SNAKE_BODY
} Color;

typedef struct {
    int x;
    int y;
} Position;

void move_snake(Position* snake, int snake_len, Direction dir) {
    // set dx and dy based on direction
    int8_t dx = 0;
    int8_t dy = 0;
    switch (dir) {
    case UP: {
        dy = -1;
        break;
    }
    case LEFT: {
        dx = -1;
        break;
    }
    case DOWN: {
        dy = 1;
        break;
    }
    case RIGHT: {
        dx = 1;
        break;
    }
    }
    for (int i = snake_len - 1; i > 0; i--) {
        snake[i] = (Position) { .y = snake[i - 1].y, .x = snake[i - 1].x };
    }
    snake[0].y += dy;
    snake[0].x += dx;
}

void get_valid_apple(Position* snake, int snake_len, int col, int row, int* outx, int* outy) {
    int test_x, test_y;
    bool is_valid = false;
    while (!is_valid) {
        is_valid = true;
        test_x = rand() % col + 1;
        test_y = rand() % row + 1;

        for (int i = 0; i < snake_len; i++) {
            if (test_x == snake[i].x && test_y == snake[i].y) {
                is_valid = false;
                break;
            }
        }
    }

    *outx = test_x;
    *outy = test_y;
}

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

    const int FRAME_DELAY = 1000 / 10;

    // create window
    int max_row, max_col;
    getmaxyx(stdscr, max_row, max_col);
    // leave 5 rows at the top, and then the border around the screen
    const int game_row = max_row - 7;
    // player is 2 wide to make it square shaped
    const int game_col = max_col / 2 - 1;
    start_color();
    WINDOW* game_window = newwin(max_row - 5, max_col, 5, 0);

    const int max_length = game_row * game_col;
    int snake_length = 3;
    Position snake[max_length];

    // input variable
    int ch;

    // player variables
    Direction player_direction = UP;
    int player_x = game_col / 2;
    int player_y = game_row / 2;
    Direction player_input = UP;

    snake[0] = (Position) { .x = player_x, .y = player_y };
    snake[1] = (Position) { .x = player_x - 1, .y = player_y };
    snake[2] = (Position) { .x = player_x - 2, .y = player_y };

    int apple_x = 1;
    int apple_y = 1;

    get_valid_apple(snake, snake_length, game_col, game_row, &apple_x, &apple_y);

    // colors
    // apple
    init_pair(APPLE, COLOR_RED, COLOR_RED);
    // snake head
    init_pair(SNAKE_HEAD, COLOR_GREEN, COLOR_GREEN);
    // snake body
    init_pair(SNAKE_BODY, COLOR_YELLOW, COLOR_YELLOW);
    // game loop
    while (1) {
        // erase last frame
        werase(game_window);

        // input
        ch = getch();
        switch (ch) {
        // up
        case KEY_UP:
        case 'W':
        case 'w': {
            player_input = 0;
            break;
        }

        // left
        case KEY_LEFT:
        case 'A':
        case 'a': {
            player_input = 1;
            break;
        }

        // down
        case KEY_DOWN:
        case 'S':
        case 's': {
            player_input = 2;
            break;
        }

        // right
        case KEY_RIGHT:
        case 'D':
        case 'd': {
            player_input = 3;
            break;
        }

        // quit
        case 'Q':
        case 'q': {
            endwin();
            return (0);
        }
        }
        // clear input buffer to remove frame lag
        while (ch != ERR) {
            ch = getch();
        }

        // if players input is perpendicular to current moving direction
        if ((((int8_t)player_input - (int8_t)player_direction) + 4) % 4 != 2) {
            player_direction = player_input;
        }

        move_snake(snake, snake_length, player_direction);
        
        // check for collisions
        if(snake[0].x == apple_x && snake[0].y == apple_y){
            snake[snake_length] = (Position){.y = snake[snake_length - 1].y, .x = snake[snake_length - 1].x};
            snake_length++;
            get_valid_apple(snake, snake_length, game_col, game_row, &apple_x, &apple_y);
        }

        // rendering
        // border
        box(game_window, 0, 0);

        // snake head
        wattron(game_window, COLOR_PAIR(SNAKE_HEAD));
        mvwprintw(game_window, snake[0].y, snake[0].x * 2, "##");
        wattroff(game_window, COLOR_PAIR(SNAKE_HEAD));

        // snake body
        wattron(game_window, COLOR_PAIR(SNAKE_BODY));
        for (int i = 1; i < snake_length; i++) {
            mvwprintw(game_window, snake[i].y, snake[i].x * 2, "##");
        }
        wattroff(game_window, COLOR_PAIR(SNAKE_BODY));

        // apple
        wattron(game_window, COLOR_PAIR(APPLE));
        mvwprintw(game_window, apple_y, apple_x * 2, "##");
        wattroff(game_window, COLOR_PAIR(APPLE));

        // update game window
        wnoutrefresh(game_window);

        // push changes to terminal screen
        doupdate();
        sleep_ms(FRAME_DELAY);
    }

    endwin();
    return (0);
}