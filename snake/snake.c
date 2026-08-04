//load curses library
#ifdef _WIN32
    //use pdcurses if on windows
    #include <curses.h>

    //use Sleep() on windows
    #include <windows.h>
    #define sleep_ms(ms) Sleep(ms);
#else
    //use ncurses if on linux
    #include <ncurses.h>

    //use usleep on linux
    #include <unistd.h>
    #define sleep_ms(ms) usleep(ms * 1000);
#endif
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

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

int main(){
    srand(time(NULL));
    //init screen
    initscr();
    //stop typed letters from showing on terminal
    noecho();
    //hide terminal cursor
    curs_set(0);
    //enable special keys (arrows, etc)
    keypad(stdscr, TRUE);
    //makes input reading non-blocking so the game runs without waiting for input
    nodelay(stdscr, TRUE);

    const int FRAME_DELAY = 1000/60;

    //create window
    int max_row, max_col;
    int game_row, game_col;
    getmaxyx(stdscr, max_row, max_col); 
    //leave 5 rows at the top, and then the border around the screen
    game_row = max_row - 7;
    //player is 2 wide to make it square shaped
    game_col = max_col/2 - 1;
    start_color();
    WINDOW* game_window = newwin(max_row - 5, max_col, 5, 0);

    int max_length = game_row * game_col;
    int snake_length = 3;
    Position snake[max_length];

    //input variable
    int ch;

    //player variables
    Direction player_direction;
    int player_x = game_col/2;
    int player_y = game_row/2;
    Direction player_input;

    snake[0] = (Position){.x = player_x, .y = player_y};
    snake[1] = (Position){.x = player_x - 1, .y = player_y};
    snake[2] = (Position){.x = player_x - 2, .y = player_y};

    //colors
    //apple
    init_pair(APPLE, COLOR_RED, COLOR_RED);
    //snake head
    init_pair(SNAKE_HEAD, COLOR_GREEN, COLOR_GREEN);
    //snake body
    init_pair(SNAKE_BODY, COLOR_YELLOW, COLOR_YELLOW);
    //game loop
    while(1){
        //erase last frame
        erase();

        //input
        ch = getch();
        switch (ch) {
            //up
            case KEY_UP:
            case 'W':
            case 'w': {
                player_input = 0;
            }

            //left
            case KEY_LEFT:
            case 'A':
            case 'a': {
                player_input = 1;
            }

            //down
            case KEY_DOWN:
            case 'S':
            case 's': {
                player_input = 2;
            }

            //right
            case KEY_RIGHT:
            case 'D':
            case 'd': {
                player_input = 3;
            }

            //quit
            case 'Q':
            case 'q': {
                endwin();
                return(0);
            }
        }
        //clear input buffer to remove frame lag
        while(ch != ERR){
            ch = getch();
        }

        //rendering
        //border
        box(game_window, 0, 0);

        //snake head
        wattron(game_window, COLOR_PAIR(SNAKE_HEAD));
        mvwprintw(game_window, player_y, player_x * 2, "##");
        wattroff(game_window, COLOR_PAIR(SNAKE_HEAD));

        //snake body
        wattron(game_window, COLOR_PAIR(SNAKE_BODY));
        for(int i = 1; i < snake_length; i++){
            mvwprintw(game_window, snake[i].y, snake[i].x * 2, "##");
        }
        wattroff(game_window, COLOR_PAIR(SNAKE_BODY));

        wrefresh(game_window);
        sleep_ms(FRAME_DELAY);
    }


    endwin();
    return(0);
}