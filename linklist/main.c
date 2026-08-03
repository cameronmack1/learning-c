#include "linklist.h"
#include <stdio.h>

int main(){
    //create 20 node
    Node* list = createNode(20);
    printf("\ncreated list with 20\n");
    printList(list);

    //add 40 to end
    list = addToEnd(list, 40);
    printf("\nadded 40 to end of list\n");
    printList(list);

    //add 10 to start
    list = addToStart(list, 10);
    printf("\nadded 10 to start of list\n");
    printList(list);

    //add 30 in position 3
    list = addAtPosition(list, 30, 2);
    printf("\nadded 30 to position 3\n");
    printList(list);

    list = deleteAtPosition(list, 1);
    printf("\ndeleted position 2\n");
    printList(list);
    return 0;
}