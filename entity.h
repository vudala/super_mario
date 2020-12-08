#ifndef ENTITY_INCLUDED
#define ENTITY_INCLUDED

#include "level.h"
#include "entity_list.h"
#include "animation.h"

#define IDLE 0
#define WALKING 1
#define JUMPING 2
#define BOUNCING 3
#define LEFT 0
#define RIGHT 1

#define BOUNCE_VELOCITY -10.0
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

typedef enum {
    NO_POWER = 0,
    MUSHROOM_POWER,
    FLOWER_POWER,
    STAR_POWER
} Powers;

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
    /* A cada frame diminui o tempo de vida da entidade, utilizado para controlar as entidades
    temporarias */
    int lifeSpan;
    int power; // Power up do personagem
    struct animation* anim; // Animação
};

// Cria uma nova entidade
struct entity* newEntity(int type, int x, int y, int w, int h, int dir, struct animation* anim, int lSpan);

// Checa colisão entre duas entidades
int entityCollision(struct entity* en1, struct entity* en2);

// Checa colisão entre duas entidades, uma para cada extremo
int entityDownCollision(struct entity* en1, struct entity* en2);
// int entityLeftCollision(struct entity* en1, struct entity* en2);
// int entityRightCollision(struct entity* en1, struct entity* en2);
// int entityUpCollision(struct entity* en1, struct entity* en2);

// Atualiza o estado de uma entidade qualquer
void updateEntity(struct entity* en, struct tile** tiles);

// Atualiza o estado do personagem principal
struct tile* updateCharacter(struct entity* character, struct tile** tiles, unsigned char* key);

// Libera o espaço de memória ocupado pela entidade
void destroyEntity(struct entity* en);

int gameUpdate(struct entity* character, struct tile** tiles, struct entityList* entities, unsigned char* key);

#endif