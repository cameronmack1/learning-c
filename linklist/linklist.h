#ifndef LINKLIST_H
#define LINKLIST_H
#include <stddef.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node* createNode(int value);
Node* addToStart(Node* list, int value);
Node* addToEnd(Node* list, int value);
Node* addAtPosition(Node* list, int value, int position);
Node* deleteStart(Node* list);
Node* deleteEnd(Node* list);
Node* deleteAtPosition(Node* list, int position);
Node* deleteWithValue (Node* list, int value);
Node* deleteFirstWithValue(Node* list, int value);
size_t length(Node* list);
bool isEmpty(Node* list);
Node* reverseList(Node* list);
void freeList(Node* list);

#endif