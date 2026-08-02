#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linklist.h"

//creating and adding to list
Node* createNode(int value){
    Node* newNode = malloc(sizeof(Node));
    //failed to allocate memory
    if(newNode == NULL){
        printf("Failed to allocate memory\n");
        exit(1);
    }
    newNode->data = value;
    newNode->next = NULL;
    return newNode; 
}

//addition functions
//add to start of list
Node* addToStart(Node* list, int value){
    //create new node and link it to the list
    Node* newNode = createNode(value);
    newNode->next = list;
    return newNode;
}

//add to end of list
Node* addToEnd(Node* list, int value){
    Node* newNode = createNode(value);
    //return new list if passed null pointer
    if(list == NULL) return newNode;
    Node* current = list;
    //loop until we reach end
    while(current->next != NULL){
        current = current->next;
    }
    //add to end
    current->next = newNode;
    return list;
}

//add node at position
Node* addAtPosition(Node* list, int value, int position){
    //create new list if passed null list
    if(list == NULL) return createNode(value);
    //add to start if position is negative or 0
    if(position <= 0) return addToStart(list, value);
    Node* newNode = createNode(value);
    Node* current = list;
    int pos = 0;
    //loop until we reach right before the position, or end of list
    while(pos < position - 1 && current->next != NULL){
        current = current->next;
        pos++;
    }
    //add node
    newNode->next = current->next;
    current->next = newNode;
    return list;
}

//deletion functions
//delete first node
Node* deleteStart(Node* list){
    if(list == NULL) return NULL;
    Node* next = list->next;
    free(list);
    return next;
}

//delete the last list
Node* deleteEnd(Node* list){
    if(list == NULL) return NULL;
    //handle list with 1 element
    if(list->next == NULL) {
        free(list);
        return NULL;
    }
    Node* current = list;
    //loop until we find second last node
    while(current->next->next!=NULL){
        current = current->next;
    }
    //free last node
    free(current->next);
    current->next==NULL;
    return list;
}

//delete node at specific position
Node* deleteAtPosition(Node* list, int position){
    if(list == NULL) return NULL;
    //delete first position if position passed is 0 or negative
    if(position <= 0){
        return deleteStart(list);
    }
    int pos = 0;
    Node* current = list;
    //loop until we reach node right before position, or we reach end of list
    while(pos < position - 1 && current->next != NULL){
        current = current->next;
        pos++;
    }
    if(current->next == NULL) return list;
    //relink nodes, and free the deleted node
    Node* toFree = current->next;
    current->next = toFree->next;
    free(toFree);
    return list;
}

//delete all nodes with specific value
Node* deleteWithValue (Node* list, int value){
    //return null if list is empty
    if(list == NULL) return NULL;
    Node* current = list;
    //handle the first element matching the value
    if(current->data == value){
        current = list->next;
        free(list);
        //use recursion to check the new list
        return deleteWithValue(current, value);
    }
    //loop until we find the value, or we reach the end of the list
    while(current->next != NULL){
        if(current->next->data == value){
            //free and relink if we find it
            Node* toFree = current->next;
            current->next = toFree->next;
            free(toFree);
            if(current->next == NULL) return list;
        } else {
            //increment if we do not find it
            current = current->next;
        }
    }
    return list;
}

//delete the first item with value
Node* deleteFirstWithValue(Node* list, int value){
    if(list == NULL) return NULL;
    Node* current = list;
    //handle first element matching value
    if(current->data == value){
        current = list->next;
        free(list);
        return current;
    }
    //loop until we find value
    while(current->next != NULL){
        if(current->next->data == value){
            Node* toFree = current->next;
            current->next = toFree->next;
            free(toFree);
            return list;
        }
        current = current->next;
    }
    return list;
}

//utility functions
//count length
size_t length(Node* list){
    Node* current = list;
    size_t length = 0;
    while(current != NULL){
        length+=1;
        current = current->next;
    }
    return length;
}

//check if its empty
bool isEmpty(Node* list){
    return list == NULL;
}

Node* reverseList(Node* list){
    //return the list if it is one or 0 elements
    if(list == NULL || list->next == NULL) return list;
    Node* prev = list;
    Node* current = list->next;
    Node* next = current->next;
    prev->next = NULL;
    //loop until we reach the end of the list
    while(current != NULL){
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    return prev;
}

//free an entire list
void freeList(Node* list){
    if(list == NULL) return;
    Node* current = list;
    Node* next;
    //loop through and free the list
    while(current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
}