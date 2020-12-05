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
#define PLAYER_WALKING_SPEED 4.0
#define MONSTER_WALKING_SPEED 2.0

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

// Atualiza o estado de uma entidade qualquer
void updateEntity(struct entity* en, struct tile** tiles);

// Atualiza o estado do personagem principal
void updateCharacter(struct entity* character, struct tile** tiles, struct entity** entities, unsigned char* key);

#endif