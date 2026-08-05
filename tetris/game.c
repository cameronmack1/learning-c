#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "game.h"

bool init(){
    Game* game = calloc(1, sizeof(Game));
    game->current.type = rand() % 7 + 1;
}