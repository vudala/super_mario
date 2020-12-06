#ifndef ENTITYLIST_INCLUDED
#define ENTITYLIST_INCLUDED

struct entityNode {
    int id;
    struct entity* en;
    struct entityNode* next;
};

struct entityList {
    struct entityNode* start;
    int size;
};

void createList(struct entityList *l);
int isEmpty(struct entityList *l);
void destroyList(struct entityList *l);
int insertEntity(struct entityList* l, struct entity* en, int* id);
int removeEntity(int id, struct entityList *l);

#endif