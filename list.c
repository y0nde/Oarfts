#include <stdio.h>
#include "list.h"

List* newList()
{
    List* plist = malloc(sizeof(List));
    plist->head = NULL;
    return plist;
}

void push_front(List* list, void* Data, int SizeofData)
{   
    //新しいNodeを予約
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = malloc(SizeofData);
    //新しいheadに今までの先頭を登録
    newNode->next = list->head;
    //第２引数からDataをコピー
    memcpy(newNode->data, Data, SizeofData);
    //リストの先頭にnewNodeを登録
    list->head = newNode;  
}

void push_back(List* list, void* Data, int SizeofData)
{
    //新しいNodeを予約
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = malloc(SizeofData);
    newNode->next = NULL;
    //第２引数からDataをコピー
    memcpy(newNode->data, Data, SizeofData);
    //リストが空の場合は先頭に新ノードを登録する。空でなければ最後のノードまで移動する。
    if(list->head == NULL)
    {
    list->head = newNode;
    }
    else
    {
    Node* pNode = list->head;
    while(pNode->next != NULL)
    {
        pNode = pNode->next;
    }
    //最後のノードのnextにnewNodeを代入
    pNode->next = newNode;
    }
}

void pop_front(List* list, void (*fptr)(void*))
{
    Node* node;
    
    if(node != NULL)
    {
        node = list->head; 
        list->head = node->next;
        fptr(node->data);
    }
}

Node* get_front(List* list){
    return list->head;
}

void freeNode(Node* pNode, void (*fptr)(void *))
{
    if(fptr == NULL)
    {
        free(pNode->data);
    }
    else
    {
        fptr(pNode->data);
    }
    free(pNode);
}

void freeList(List* list, void (*fptr)(void *))
{
    Node* pNode = list->head;
    Node* tmp;
    while(pNode != NULL)
    {
        tmp = pNode;
        pNode = pNode->next;
        freeNode(tmp, fptr);
    }
}

void printList(List* list, void (*fptr)(void *))
{
    Node* pNode = list->head;
    if(pNode == NULL){
        printf("empty\n");
    }
    while(pNode != NULL)
    {
        fptr(pNode->data);
        pNode = pNode->next;
    }
}

void printInt(void *n)
{
    printf(" %d\n",*(int*)n);
}

void printStr(void *n)
{
    printf(" %s\n",(char*)n);
}

int length(List* list)
{
    int cnt = 0;
    Node* pNode = list->head;
    while(pNode != NULL)
    {
    cnt++;
    pNode = pNode->next;
    }
    return cnt;
}

