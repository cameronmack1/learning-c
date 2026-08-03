#include "vector.h"
#include <stdio.h>

int main(){
    //create list with 3 spaces
    Vector* list = init(3);
    if(list == NULL)
        return 1;
    
    // add some values and print every time
    push(list, 100);
    printf("added 100\n");
    printf("capacity: %zu size: %zu\n", list->capacity, list->size);

    push(list, 30);
    printf("added 30\n");
    printf("capacity: %zu size: %zu\n", list->capacity, list->size);

    push(list, 10000);
    printf("added 10000\n");
    printf("capacity: %zu size: %zu\n", list->capacity, list->size);

    push(list, 4);
    printf("added 4\n");
    printf("capacity: %zu size: %zu\n", list->capacity, list->size);

    push(list, 80);
    printf("added 8\n");
    printf("capacity: %zu size: %zu\n", list->capacity, list->size);

    return 0;
}