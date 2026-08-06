#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "game.h"
#include "pieces.h"

void spawn_next_piece(Game* game){
    //spawn a new piece into game->next
    //generate random piece
    game->next.type = rand() % 7 + 1;
    //copy from pieces to next
    memcpy(game->next.shape, piece_shapes[game->next.type], sizeof(game->next.shape));
}

void to_next_piece(Game* game){
    //move next piece to current and spawn the next piece
    //move next to current
    memcpy(&game->current, &game->next, sizeof(game->next));
    //spawn next one
    spawn_next_piece(game);
}

bool init(Game** out){
    //initialize a game board, and pass to out pointer

    //allocate memory and initialize to 0
    Game* game = calloc(1, sizeof(Game));
    if(game == NULL){
        return false;
    }
    
    //create new block
    game->current.type = rand() % 7 + 1;
    memcpy(game->current.shape, piece_shapes[game->current.type], sizeof(game->current.shape));

    game->next.type = rand() % 7 + 1;

    //pass output pointer
    *out = game;
    return true;
}