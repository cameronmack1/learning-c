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

void render_background(int pixel_size, int start_x, int start_y) {
    int outer_width = pixel_size / 3;

    // draw rectangle
    Rectangle rec = {
        start_x - outer_width,
        start_y - outer_width,
        10 * pixel_size + 2 * outer_width,
        20 * pixel_size + 2 * outer_width
    };
    DrawRectangleRounded(rec, 0.05f, 4, GRAY);
}

void render_score(int pixel_size, int start_x, int start_y, int score, int lines) {
    int outer_width = pixel_size / 3;

    // draw outline
    Rectangle rec = {
        start_x,
        start_y - outer_width,
        7 * pixel_size + 2 * outer_width,
        5 * pixel_size + 2 * outer_width
    };
    DrawRectangleRounded(rec, 0.05f, 4, GRAY);

    // clear inside
    DrawRectangle(start_x + outer_width, start_y, 7 * pixel_size, 5 * pixel_size, BLACK);

    // draw text
    DrawText(TextFormat("Score: %d", score), start_x + 2 * outer_width, start_y + outer_width, pixel_size, WHITE);

    DrawText(TextFormat("Lines: %d", lines), start_x + 2 * outer_width, start_y + outer_width * 9, pixel_size, WHITE);
}

void render_next(int pixel_size, int start_x, int start_y, const Piece* piece) {
    int outer_width = pixel_size / 3;

    // draw outline
    Rectangle rec = {
        start_x,
        start_y + 5 * pixel_size,
        7 * pixel_size + 2 * outer_width,
        7 * pixel_size + 2 * outer_width
    };
    DrawRectangleRounded(rec, 0.05f, 4, GRAY);

    // clear inside
    DrawRectangle(start_x + outer_width, start_y + 5 * pixel_size + outer_width, 7 * pixel_size, 7 * pixel_size, BLACK);

    // text
    DrawText(TextFormat("Next:"), start_x + 2 * pixel_size, start_y + 5 * pixel_size + outer_width, pixel_size, WHITE);

    int block_start_x = start_x + 2 * pixel_size;
    int block_start_y = start_y + 7 * pixel_size;

    // loop over blocks shape
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (!piece->shape[j][i])
                continue;
            Rectangle rec = {
                block_start_x + pixel_size * i,
                block_start_y + pixel_size * j,
                pixel_size,
                pixel_size
            };

            DrawRectangleRec(rec, get_piece_color(piece->type));
            DrawRectangleLinesEx(rec, 2, Fade(WHITE, 0.5f));
        }
    }
}

void render_gameover(int score, int high_score, int start_x, int start_y, int size) {
    // create rec
    Rectangle rec = {
        start_x,
        start_y,
        size * 10,
        size * 6
    };

    // draw rec
    DrawRectangleRec(rec, BLACK);

    DrawRectangleRoundedLinesEx(rec, 0.1, 10, size / 3, RED);

    // text
    DrawText(TextFormat("Game Over"), start_x + size, start_y + size, size, RED);

    DrawText(TextFormat("Score: %d", score), start_x + size, start_y + 3 * size, size, RED);

    DrawText(TextFormat("Press ENTER to reset"), start_x + 2 * size, start_y + 5 * size, size / 2, GRAY);
}

void render_board(const Game* game, int size, int start_x, int start_y) {
    // render board
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            Rectangle rec = {
                i * size + start_x,
                j * size + start_y,
                size,
                size
            };
            DrawRectangleRec(rec, get_piece_color(game->board[j][i]));
            if (game->board[j][i]) {
                DrawRectangleLinesEx(rec, 2, Fade(WHITE, 0.5f));
            }
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
                if ((game->current.y + j) >= 0) {
                    // create rectangle
                    Rectangle rec = {
                        start_x + cur_x + size * i,
                        start_y + cur_y + size * j,
                        size,
                        size
                    };

                    DrawRectangleRec(rec, get_piece_color(game->current.type));
                    DrawRectangleLinesEx(rec, 2, Fade(WHITE, 0.5f));
                }
            }
        }
    }
}

void render_game(const Game* game, int high_score) {
    int screen_width = GetScreenWidth();
    int screen_height = GetScreenHeight();

    int pixel_size = screen_height / 25;

    int board_start_x = screen_width / 2 - (pixel_size * 5);
    int board_start_y = screen_height / 2 - (pixel_size * 10);

    render_background(pixel_size, board_start_x, board_start_y);

    int score_start_x = board_start_x + pixel_size * 10;
    int score_start_y = board_start_y + pixel_size * 2;

    render_score(pixel_size, score_start_x, score_start_y, game->score, game->lines);

    render_next(pixel_size, score_start_x, score_start_y, &game->next);

    render_board(game, pixel_size, board_start_x, board_start_y);

    if (game->game_over) {
        int go_start_x = screen_width / 2 - 5 * pixel_size;
        int go_start_y = screen_height / 2 - 3 * pixel_size;

        render_gameover(game->score, high_score, go_start_x, go_start_y, pixel_size);
    }
}