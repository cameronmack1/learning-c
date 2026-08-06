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
    bool shape[4][4];
    int x;
    int y;
    PieceType type;
} Piece;

typedef struct {
    PieceType board[20][10];
    Piece current;
    Piece next;
    int score;
    int lines;
    bool game_over;
    int tick_counter;
    int fall_tick;
} Game;

#endif