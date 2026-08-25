#include <raylib.h>
#include <stdbool.h>
#include <stddef.h>

#include "game.h"
#include "pieces.h"
#include "render.h"

// get the color of a piece from its type
Color get_piece_color(PieceType type) {
    switch (type) {
    case PIECE_NOTHING:
        return BLACK;
    case PIECE_I:
        return SKYBLUE;
    case PIECE_O:
        return YELLOW;
    case PIECE_T:
        return PURPLE;
    case PIECE_S:
        return GREEN;
    case PIECE_Z:
        return RED;
    case PIECE_J:
        return BLUE;
    case PIECE_L:
        return ORANGE;
    }
    return WHITE;
}

void render_game(const Game* game, size_t size, int start_x, int start_y ) {
    // render board
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            DrawRectangle(i * size * start_x, j * size + start_y, size, size, get_piece_color(game->board[j][i]));
        }
    }

    // render current piece and ghost
    int cur_x = game->current.x * size;
    int cur_y = game->current.y * size;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // if part of the shape
            if (game->current.shape[j][i]) {
                // draw ghost
                DrawRectangle(start_x + cur_x + size * i, start_y + cur_y + size * (j + game->ghost_dy), size, size, WHITE);

                // skip drawing if above roof
                if ((game->current.y + i) < 0)
                    continue;

                DrawRectangle(start_x + cur_x + size * i, start_y + cur_y + size * j, size, size, get_piece_color(game->current.type));
            }
        }
    }
}