#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef enum {
    ROCK, 
    PAPER, 
    SCISSORS,
    UNKNOWN
} Choices;

const char* State[] = {
    "Rock",
    "Paper",
    "Scissors"
};

//convert input to choice
Choices HandleChoice(char *str){
    if(str == NULL) return UNKNOWN;
    if(*str == 'r') return ROCK;
    if(*str == 'p') return PAPER;
    if(*str == 's') return SCISSORS;
    return UNKNOWN;
}

int main(int argc, char *argv[]){
    srand(time(NULL));
    if(argc < 2){
        printf("invalid argument, expected 1\n");
        return 1;
    }
    //determine player choice
    Choices playerChoice = HandleChoice(argv[1]);
    if(playerChoice == UNKNOWN){
        printf("Player choice could not be determined, please enter r, p, or s\n");
        return 1;
    }
    //pick random choice for computer
    Choices computerChoice = rand() % 3;
    //determine outcome
    switch(playerChoice - computerChoice){
        case 0: {
            //tie
            printf("computer picked %s, you tie\n", State[computerChoice]);
            break;
        }
        case -2:
        case 1: {
            //player win
            printf("computer picked %s, you win!\n", State[computerChoice]);
            break;
        }
        case -1:
        case 2: {
            //player lose
            printf("computer picked %s, you lose :(\n", State[computerChoice]);
            break;
        }
    }
    return 0;
}