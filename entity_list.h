#ifndef ENTITYLIST_INCLUDED
#define ENTITYLIST_INCLUDED

struct entityNode {
    int id; // Chave de identificação
    struct entity* en; // Valor
    struct entityNode* next; // Pŕoximo nodo
};

struct entityList {
    int idTrack; // Contador para ID's
    struct entityNode* start; // Primeiro nodo da lista
    int size; // Tamanho da lista
};

void createList(struct entityList *l);
int isEmpty(struct entityList *l);
void destroyList(struct entityList *l);
int insertEntity(struct entityList* l, struct entity* en);
int removeEntity(int id, struct entityList *l);

#endif