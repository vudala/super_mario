#ifndef ENTITY_INCLUDED
#define ENTITY_INCLUDED

struct tile;
#include "game.h"
#include "animation.h"
#include "entity_list.h"

#define IDLE 0
#define WALKING 1
#define JUMPING 2
#define LEFT 0
#define RIGHT 1

#define JUMP_VELOCITY -15.0;
#define GRAVITY 0.5
#define PLAYER_WALKING_SPEED 4.0
#define MONSTER_WALKING_SPEED 2.0

#define MAX_ENEMIES 50

typedef enum {
    MAIN_CHARACTER = 'X',
    GOOMBA = 'g',
    TURTLE = 't',
    FLOWER = 'F',
    STAR = 'S',
    SHELL = 'D',
    MUSHROOM = 'M',
    COIN = 'C'
} EntityTipe;

struct entity {
    int type; // Tipo de entidade
    int x; // Posicao X
    int y; // Posicao Y
    float dx; // Velocidade X
    float dy; // Velocidade Y
    int dir; // Direção
    int w; // Largura
    int h; // Altura
    int behavior; // Atual comportamento da entidade
    int life; // Pontos de vida
    struct animation* anim; // Animação
};

// Cria uma nova entidade
struct entity* newEntity(int type, int x, int y, int w, int h, int dir, int behavior, struct animation* anim);

// Checa colisão entre duas entidades
int entityCollision(struct entity* en1, struct entity* en2);

// Checa colisão entre duas entidades, uma para cada extremo
int entityLeftCollision(struct entity* en1, struct entity* en2);
int entityRightCollision(struct entity* en1, struct entity* en2);
int entityDownCollision(struct entity* en1, struct entity* en2);
int entityUpCollision(struct entity* en1, struct entity* en2);

// Atualiza o estado de uma entidade qualquer
void updateEntity(struct entity* en, struct tile** tiles);

// Atualiza o estado do personagem principal
void updateCharacter(struct entity* character, struct tile** tiles, struct entityList* entities, unsigned char* key);

// Libera o espaço de memória ocupado pela entidade
void destroyEntity(struct entity* en);

#endif