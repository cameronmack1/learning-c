#include "game.h"
#include "pieces.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

<<<<<<< HEAD
#define SPAWN_X 3;
#define SPAWN_Y -1;

#define MAX_X 10;
#define MAX_Y 20;
=======
#define SPAWN_X 3
#define SPAWN_Y -1

#define MAX_X 10
#define MAX_Y 20
>>>>>>> 7a44b3f (add logic to check if a block should lock into place)

void spawn_next_piece(Game* game) {
    // spawn a new piece into game->next
    // generate random piece
    game->next.type = rand() % 7 + 1;
    // copy from pieces to next
    memcpy(game->next.shape, piece_shapes[game->next.type], sizeof(game->next.shape));
}

void to_next_piece(Game* game) {
    // move next piece to current and spawn the next piece
    // move next to current
    memcpy(&game->current, &game->next, sizeof(game->next));
    game->current.x = SPAWN_X;
    game->current.y = SPAWN_Y;
    // spawn next one
    spawn_next_piece(game);
}

<<<<<<< HEAD
bool check_lock(Game* game, Piece piece) {
    // checks if a piece should lock when moving down
}

=======
bool check_lock(Game* game, Piece* piece) {
    // checks if a piece should lock when moving down
    // loop thru pieces shape grid
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // check if the grid space is occupied
            if (!piece->shape[i][j])
                continue;

            // compare the spot below a piece to the game board and floor
            int ny = piece->y + i + 1;
            int nx = piece->x + j;

            if (ny >= MAX_Y)
                return true;
            //make sure the pos is valid, and check if that spot on the board is occupied
            if (nx >= 0 && nx < MAX_X && ny >= 0 && game->board[ny][nx]) {
                return true;
            }
        }
    }
    return false;
}

>>>>>>> 7a44b3f (add logic to check if a block should lock into place)
bool check_collision(Game* game, int dx) {
    // checks if a piece can move left/right
}

<<<<<<< HEAD
bool drop_block(Piece* piece) {
    // returns true if the block is hitting the ground, else moves it down a block
=======
bool drop_block(Game* game, Piece* piece) {
    // returns true if the block is hitting the ground, else moves it down a block
    if (check_lock(game, piece))
}

int hard_fall_block(Game* game) {
    // returns how many blocks for the current piece to fall for it to lock
>>>>>>> 7a44b3f (add logic to check if a block should lock into place)
}

bool init(Game** out) {
    // initialize a game board, and pass to out pointer

    // allocate memory and initialize to 0
    Game* game = calloc(1, sizeof(Game));
    if (game == NULL) {
        return false;
    }

    // create new block
    spawn_next_piece(game);
    to_next_piece(game);

    // pass output pointer
    *out = game;
    return true;
}

void tick(Game* game, bool input[6]) {
    // tick the game
    // check player input
    // left, right, rotate cw, rotate ccw, hard drop, soft drop

    int dx = input[1] - input[0]; // left and right cancel out
    if (dx != 0) {
        // if one of left or right is being pressed
        if (!check_collision(game, dx)) {
            // if we can move there
            game->current.x += dx;
        }
    }
<<<<<<< HEAD
    
    //fall 1 space per tick if fast falling, else 1 space per 20 ticks (3/sec)
=======

    // fall 1 space per tick if fast falling, else 1 space per 20 ticks (3/sec)
>>>>>>> 7a44b3f (add logic to check if a block should lock into place)
    game->fall_tick = input[5] ? 1 : 20;

    // block dropping
    if (game->tick_counter > game->fall_tick) {
        if (drop_block(game)) {
            // if block has fallen down into a block, then lock it
            to_next_piece(game);
        }
    }

    game->tick_counter++;
}