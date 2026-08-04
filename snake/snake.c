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

typedef enum EDirection{
    UP,
    LEFT,
    DOWN,
    RIGHT
} Direction;

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
    int max_row, max_col;
    getmaxyx(stdscr, max_row, max_col); 

    //input variable
    int ch;

    //player variables
    Direction player_direction;
    int player_x;
    int player_y;
    Direction player_input;

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

        refresh();
        sleep_ms(FRAME_DELAY);
    }


    endwin();
    return(0);
}