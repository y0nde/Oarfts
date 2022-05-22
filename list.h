#pragma once

#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    void* data;
    struct Node* next;
} Node;

typedef struct List
{
    Node* head;
} List;

List* newList();

void push_front(List* list, void* Data, int SizeofData);

void push_back(List* list, void* Data, int SizeofData);

void pop_front(List* list, void (*fptr)(void*));

Node* get_front(List* list);

void removeNode(List* list,int index);

void freeList(List* list, void (*fptr)(void *));

void printList(List* list, void (*fptr)(void *));

void printInt(void*);

void printStr(void*);

int length(List* list);


