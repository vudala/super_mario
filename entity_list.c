#include <stdio.h>
#include <stdlib.h>
#include "entity_list.h"

void createList(struct entityList* l){
    l->size = 0;
    l->start = NULL;
}

int isEmpty(struct entityList* l){
    if(l->size == 0) return 1;
    return 0;
}

void destroyList(struct entityList* l){
    struct entityNode* next = l->start;
    while(l->start != NULL){
        l->start = next->next;        
        free(next);
        next = l->start;
    }
    l->size = 0;
    l->start = NULL;
}

int insertEntity(struct entityList* l, struct entity* en, int* id){
    struct entityNode* nodo = malloc(sizeof(struct entityNode));
    if(nodo == NULL) return 0;
    nodo->id = *id;
    nodo->en = en;
    nodo->next = NULL;

    if( isEmpty(l) ){
        l->start = nodo;
        l->size++;
        return 1;
    }

    struct entityNode* current = l->start;
    
    while(current->next != NULL) current = current->next;
    current->next = nodo;
    l->size++;

    return 1;
}

int removeFirst(struct entityList* l){
    if(!isEmpty(l)){
        struct entityNode* current = l->start->next;
        free(l->start);
        l->start = current;
        l->size--;
        return 1;
    }
    return 0;
}

int removeEntity(int id, struct entityList *l){
    if(isEmpty(l)) return 0;
  
    if(l->start->id == id) return removeFirst(l);

    struct entityNode* current = l->start;
    struct entityNode* last = NULL;
    while(current != NULL){
        if(current->id == id){
            last->next = current->next;
            free(current);
            l->size--;
            return 1;
        }
        last = current;
        current = current->next;
    }
    return 0;
}