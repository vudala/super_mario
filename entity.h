#ifndef ENTITY_INCLUDED
#define ENTITY_INCLUDED

struct tile;

#define IDLE 0
#define WALKING 1
#define JUMPING 2
#define LEFT 0
#define RIGHT 1

typedef enum {
    MAIN_CHARACTER = 'X',
    GOOMBA = 'g',
    TURTLE = 't'
} EntityTipe;

struct entity {
    int x; // Posicao X
    int y; // Posicao Y
    float dx; // Velocidade X
    float dy; // Velocidade Y
    int dir; // Direção
    int w; // Largura
    int h; // Altura
    int behavior; // Atual comportamento da entidade
};

// Cria uma nova entidade
struct entity* newEntity(int x, int y, int dx, int dy, int dir, int w, int h, int behavior);

// Checa se há colisão acima da entidade
int checkUpCollision(struct entity* en, struct tile** tiles);

// Checa se há colisão abaixo da entidade
int checkDownCollision(struct entity* en, struct tile** tiles);

// Checa se há colisão à esquerda da entidade
int checkLeftCollision(struct entity* en, struct tile** tiles);

// Checa se há colisao à direita da entidade
int checkRightCollision(struct entity* en, struct tile** tiles);

// Aponta para a tile que se encontra naquele determinado par de coordenadas
struct tile* pointToTile(int x, int y, struct tile** tiles);

#endif