// GRR20195689 Eduardo Vudala Senoski

#ifndef ENTITYLIST_INCLUDED
#define ENTITYLIST_INCLUDED

struct entityNode {
    int id; // Chave de identificação
    struct entity* en; // Entidade
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

/* Itera sobre a lista e aplica uma função envolvendo duas entidades, retorna o id do nodo
no qual a função aplicada sobre en e um nodo qualquer da lista retorna true,
caso nenhuma delas retorne true, retorna -1*/
int listIterate(int (func) (struct entity*, struct entity*), struct entityList* l, struct entity* en);

#endif