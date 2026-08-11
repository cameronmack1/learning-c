#ifndef GAME_H
#define GAME_H
#include <stdbool.h>

typedef enum {
    PIECE_NOTHING,
    PIECE_I,
    PIECE_T,
    PIECE_O,
    PIECE_S,
    PIECE_Z,
    PIECE_J,
    PIECE_L
} PieceType;

typedef struct {
    int x;
    int y;
    PieceType type;
    bool shape[4][4];
} Piece;

typedef struct {
    Piece current;
    Piece next;
    PieceType board[20][10];
    int score;
    int lines;
    int tick_counter;
    int fall_tick;
    bool game_over;
    //vvvvv isnt useful in ncurses, but is if moved out of ncurses later
    bool holding_hard;
} Game;

void tick(Game* game, bool input[6]);
bool init(Game** out);
void spawn_next_piece(Game* game); 
bool to_next_piece(Game* game);

#endif