#ifndef ENTITY_INCLUDED
#define ENTITY_INCLUDED

struct tile;
#include "game.h"

#define IDLE 0
#define WALKING 1
#define JUMPING 2
#define LEFT 0
#define RIGHT 1

#define JUMP_VELOCITY -10.0;
#define GRAVITY 0.5
#define WALKING_SPEED 2.0

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
    ALLEGRO_BITMAP** animation;  // [0] idle [1] walk1 [2] walk2 [3] jump
};

// Cria um conjunto de animções baseados em arquivos providos
ALLEGRO_BITMAP** newAnimation(char* idlePath, char* walk1Path, char* walk2Path, char* jumpPath);

// Cria uma nova entidade
struct entity* newEntity(int x, int y, int dx, int dy, int dir, int w, int h, int behavior, ALLEGRO_BITMAP** animation);

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

// Atualiza o estado do personagem principal
void updateCharacter(struct entity* character, struct tile** tiles, unsigned char* key);

// Desenha uma entidade qualquer utilizando um deslocamento
void drawEntity(struct entity* en, int offset);

#endif