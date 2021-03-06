#ifndef ENTITY_INCLUDED
#define ENTITY_INCLUDED

#include "level.h"
#include "entity_list.h"
#include "animation.h"

typedef enum {
    IDLE = 0,
    WALKING,
    JUMPING,
    BOUNCING
} Behaviors;

#define LEFT 0 // Aponta para a esquerda
#define RIGHT 1 // Aponta para a direita

#define GRAVITY 0.5

#define FAST_SPEED 8.0
#define SLOW_SPEED 3.0

#define HIGH_BOUNCE -12.0
#define LOW_BOUNCE -6.0

#define FIREBALL_SIZE 20
#define FIREBALL_SPAN 120
#define INFINITE -1

#define COIN_SCORE 100 // Pontos ganhos para cada moeda que pegar
#define KILL_SCORE 250 // Pontos ganhos para cada inimigo que matar
#define BUFF_SCORE 500 // Pontos ganhos quando recebe um buff repetido ou menos poderoso que o atual

typedef enum {
    MAIN_CHARACTER = 'X',
    MAIN_SMALL = 'x',
    MAIN_FLOWER = 'K',
    GOOMBA = 'g',
    TURTLE = 't',
    FLOWER = 'F',
    STAR = 'S',
    SHELL = 'D',
    MUSHROOM = 'M',
    COIN = 'C',
    FIREBALL = 'B',
    CHECKPOINT = 'E'
} EntityTipe;

typedef enum {
    NO_POWER = 0,
    MUSHROOM_POWER = MUSHROOM,
    FLOWER_POWER = FLOWER,
    STAR_POWER = STAR
} Powers;

#define HIT_SPAN 30 // Frames invencíveis após tomar um hit
#define INVINCIBLE_SECONDS 10 // Quantos segundos deve ficar invencível pelo poder da estrela
#define INVINCIBILTY (INVINCIBLE_SECONDS * FPS) // Frames invencíveis do pode da estrela

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
    struct animation* anim; // Animação
};


// Libera o espaço de memória ocupado pela entidade
void destroyEntity(struct entity* en);

// Cria uma nova entidade
struct entity* newEntity(int type, int x, int y, int w, int h, int dir, struct animation* anim, int lSpan);

// Verifica se uma entidade está morta
int isDead(struct entity* en);

// Checa colisão entre duas entidades
int entityCollision(struct entity* en1, struct entity* en2);

// Checa colisão com a entidade de baixo
int entityDownCollision(struct entity* en1, struct entity* en2);

// Adiciona uma fireball a frente de en
void addFireball(struct entityList* fireballs, struct entity* en);

// Verifica se alguma fireball bateu na entidade
int fireballHit(struct entityList* fireballs, struct entity* en);

// Verifica se o casco colidiu com essa entidade
int shellHit(struct entity* shell, struct entity* en);

// Atualiza o estado e posição das fireballs
void fireballsUpdate(struct entityList* fireballs, struct tile** tiles);

// Atualiza o estado de uma entidade qualquer
void updateEntity(struct entity* en, struct tile** tiles);

#endif