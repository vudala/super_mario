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

// Cria uma nova lista
struct entityList* newList();

// Verifica se a lista está vazia
int isEmpty(struct entityList *l);

// Destroi todos os nodos da lista
void destroyList(struct entityList *l);

// Insere um novo nodo na lista
int insertEntity(struct entityList* l, struct entity* en);

// Remove um nodo da lista
int removeEntity(int id, struct entityList *l);

#endif