#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Invalid arguments, expected one argument\n");
        return 1;
    }
    long long num1, num2, result;
    char operator;
    int parsed = sscanf(argv[1], "%lld %c %lld", &num1, &operator, &num2);
    if(parsed != 3){
        printf("Invalid argument, correct example: '123+321'\n");
        return 1;
    }
    switch(operator){
        case '+':{
            result = num1 + num2;
            break;
        }
        case '-':{
            result = num1 - num2;
            break;
        }
        case '*':{
            result = num1 * num2;
            break;
        }
    }
    printf("Result: %lld\n", result);
    return 0;
}