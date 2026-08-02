#include "linklist.h"

int main(){
    Node* list = createNode(20);
    list = addToEnd(list, 40);
    list = addToStart(list, 10);
    list = addAtPosition(list, 30, 2);
    list = printList(list);
}