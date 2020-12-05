#ifndef ENTITY_INCLUDED
#define ENTITY_INCLUDED

struct tile;
#include "game.h"
#include "animation.h"

#define IDLE 0
#define WALKING 1
#define JUMPING 2
#define LEFT 0
#define RIGHT 1

#define JUMP_VELOCITY -15.0;
#define GRAVITY 0.5
#define WALKING_SPEED 4.0

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
    struct animation* anim; // Animação
};

// Cria uma nova entidade
struct entity* newEntity(int x, int y, int dir, int behavior, struct animation* anim);

// Checa se há colisão acima da entidade
int checkUpCollision(struct entity* en, struct tile** tiles);

// Checa se há colisão abaixo da entidade
int checkDownCollision(struct entity* en, struct tile** tiles);

// Checa se há colisão à esquerda da entidade
int checkLeftCollision(struct entity* en, struct tile** tiles);

// Checa se há colisao à direita da entidade
int checkRightCollision(struct entity* en, struct tile** tiles);

// Atualiza o estado de uma entidade qualquer
void updateEntity(struct entity* en, struct tile** tiles);

// Atualiza o estado do personagem principal
void updateCharacter(struct entity* character, struct tile** tiles, unsigned char* key);

#endif