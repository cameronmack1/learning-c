#include "game.h"
#include "pieces.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SPAWN_X 3
#define SPAWN_Y -1

#define MAX_X 10
#define MAX_Y 20

void reset_bag(Game* game) {
    // reset
    game->piece_bag.cur_index = 0;
    for (int i = 0; i < 7; i++) {
        game->piece_bag.bag[i] = i + 1;
    }

    // fisher-yates shuffle
    int temp;
    int pos;
    for (int i = 6; i > 0; i--) {
        // generate random position to swap with
        pos = rand() % (i + 1);

        // swap
        temp = game->piece_bag.bag[i];
        game->piece_bag.bag[i] = game->piece_bag.bag[pos];
        game->piece_bag.bag[pos] = temp;
    }
}

void spawn_next_piece(Game* game) {
    // spawn a new piece into game->next
    // generate random piece
    game->next.type = game->piece_bag.bag[game->piece_bag.cur_index++];
    if (game->piece_bag.cur_index >= 7)
        reset_bag(game);
    // copy from pieces to next
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            game->next.shape[i][j] = piece_shapes[game->next.type][i][j];
        }
    }
}

bool to_next_piece(Game* game) {
    // returns true if game over
    // move next piece to current and spawn the next piece
    // add current to board
    int x_pos = game->current.x;
    int y_pos = game->current.y;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (game->current.shape[i][j]) {
                int ny = y_pos + i;
                int nx = x_pos + j;
                // game over if a piece locks above top of map
                if (ny < 0)
                    return true;

                if (nx < 0 || nx >= MAX_X || ny >= MAX_Y) // shouldnt happen
                    continue;
                // set board value
                game->board[ny][nx] = game->current.type;
            }
        }
    }
    // move next to current
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            game->current.shape[i][j] = game->next.shape[i][j];
        }
    }
    game->current.type = game->next.type;
    game->current.x = SPAWN_X;
    game->current.y = SPAWN_Y;
    // spawn next one
    spawn_next_piece(game);
    return false;
}

bool check_collision(Game* game, Piece* piece, int dx, int dy) {
    // checks if a piece can move by dx and dy
    // loop thru pieces shape grid
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // check if the grid space is occupied
            if (!piece->shape[i][j])
                continue;

            // compare the spot below a piece to the game board and floor
            int ny = piece->y + i + dy;
            int nx = piece->x + j + dx;

            if (ny >= MAX_Y || nx >= MAX_X || nx < 0)
                return true;
            // make sure the pos is valid, and check if that spot on the board is occupied
            if (ny >= 0 && game->board[ny][nx]) {
                return true;
            }
        }
    }
    return false;
}

bool drop_block(Game* game, Piece* piece) {
    // returns true if the block is hitting the ground, else moves it down a block
    if (check_collision(game, piece, 0, 1)) {
        return true;
    }
    piece->y++;
    return false;
}

void transpose(Piece* piece) {
    // swaps rows and columns
    int piece_size = piece_sizes[piece->type];
    bool temp;
    for (int i = 0; i < piece_size; i++) {
        for (int j = i + 1; j < piece_size; j++) {
            // swap
            temp = piece->shape[i][j];
            piece->shape[i][j] = piece->shape[j][i];
            piece->shape[j][i] = temp;
        }
    }
}

void reverse_row(Piece* piece) {
    int piece_size = piece_sizes[piece->type];
    bool temp;
    // loop over every row
    for (int i = 0; i < piece_size; i++) {
        // loop thru half of the row
        for (int j = 0; j < piece_size / 2; j++) {
            temp = piece->shape[i][j];
            piece->shape[i][j] = piece->shape[i][piece_size - 1 - j];
            piece->shape[i][piece_size - 1 - j] = temp;
        }
    }
}

void rotate_cw(Piece* piece) {
    // transpose, then reverse each row
    // O piece does not rotate
    if (piece->type == PIECE_O)
        return;
    transpose(piece);
    reverse_row(piece);
}

void rotate_ccw(Piece* piece) {
    // reverse each row, then transpose
    // O piece does not rotate
    if (piece->type == PIECE_O)
        return;
    reverse_row(piece);
    transpose(piece);
}

int hard_fall_block(Game* game) {
    // returns how many blocks for the current piece to fall for it to lock
    int counter = 0;
    while (!check_collision(game, &game->current, 0, ++counter)) {
        // no body
    }
    return counter - 1;
}

bool init(Game** out) {
    // initialize a game board, and pass to out pointer

    // allocate memory and initialize to 0
    Game* game = calloc(1, sizeof(Game));
    if (game == NULL)
        return false;

    // initialize piece bag
    reset_bag(game);
    // create new block
    spawn_next_piece(game);
    // move next block to current
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            game->current.shape[i][j] = game->next.shape[i][j];
        }
    }
    // set pos
    game->current.type = game->next.type;
    game->current.x = SPAWN_X;
    game->current.y = SPAWN_Y;
    // create next
    spawn_next_piece(game);

    // pass output pointer
    *out = game;
    return true;
}

bool wall_kick(Game* game, Piece* piece) {
    // move and returnt true if possible value found, return false otherwise
    // O piece does not change when rotating
    int kick_values[5][2];
    // different kick values for different pieces
    if (piece->type == PIECE_O)
        return true;
    if (piece->type == PIECE_I) {
        memcpy(kick_values, line_kick_values, sizeof(kick_values));
    } else {
        memcpy(kick_values, reg_kick_values, sizeof(kick_values));
    }
    // check each kick value
    for (int i = 0; i < 5; i++) {
        if (!check_collision(game, piece, kick_values[i][0], kick_values[i][1])) {
            piece->x += kick_values[i][0];
            piece->y += kick_values[i][1];
            return true;
        }
    }
    return false;
}

void tick(Game* game, bool input[6]) {
    // tick the game
    // check player input
    // left, right, rotate cw, rotate ccw, hard drop, soft drop

    // handle rotation
    if (input[2]) {
        rotate_cw(&game->current);
        if (!wall_kick(game, &game->current))
            rotate_ccw(&game->current);
    } else if (input[3]) {
        rotate_ccw(&game->current);
        if (!wall_kick(game, &game->current))
            rotate_cw(&game->current);
    }

    // handle left-right input
    int dx = input[1] - input[0]; // left and right cancel out
    if (dx != 0) {
        // if one of left or right is being pressed
        if (!check_collision(game, &game->current, dx, 0)) {
            // if we can move there
            game->current.x += dx;
        }
    }

    // fall 1 space per tick if fast falling, else 1 space per 20 ticks (3/sec)
    game->fall_tick = input[5] ? 1 : 20;

    // block dropping
    if (game->tick_counter >= game->fall_tick) {
        // reset tick counter to 0
        game->tick_counter = 0;
        if (drop_block(game, &game->current)) {
            // if block has fallen down into a block, then lock it
            game->game_over = to_next_piece(game);
        }
    }

    game->ghost_dy = hard_fall_block(game);

    if (input[4] && !game->holding_hard) {
        // reset tick counter
        game->tick_counter = 0;
        // prevent next block from dropping instantly
        game->holding_hard = true;
        // drop piece
        game->current.y += game->ghost_dy;
        // lock
        game->game_over = to_next_piece(game);
    }
    game->holding_hard = input[4];

    // check for lines
    int num_lines = 0;
    for (int i = 19; i >= 0; i--) {
        for (int j = 0; j < 10; j++) {
            // break to next if there is a hole
            if (game->board[i][j] == 0)
                break;
            // if we reached the end of the line, then it is full
            if (j == 9) {
                num_lines++;
                // shift every row down
                for (int k = i; k > 0; k--) {
                    memcpy(game->board[k], game->board[k - 1], sizeof(game->board[k]));
                }
                // wipe top row
                memset(game->board[0], 0, sizeof(game->board[0]));

                // check current row again cuz theres something new there
                i++;
            }
        }
    }
    switch (num_lines) {
    case 1: {
        game->score += 100;
        break;
    }
    case 2: {
        game->score += 300;
        break;
    }
    case 3: {
        game->score += 500;
        break;
    }
    case 4: {
        game->score += 800;
        break;
    }
    }
    game->lines += num_lines;

    game->tick_counter++;
}