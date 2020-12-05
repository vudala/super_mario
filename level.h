#ifndef LEVEL_INCLUDED
#define LEVEL_INCLUDED

struct entity;

typedef enum {
    EMPTY_BLOCK = ' ',
    BRICK_BLOCK = 'b',
    HARD_BRICK_BLOCK = 'h',
    PIPE_BLOCK = 'p',
    PIPE_TOP_BLOCK = 'P',
    COIN_BLOCK = 'c',
    STAR_BLOCK = 's',
    MUSHROOM_BLOCK = 'm',
    FLOWER_BLOCK = 'f'
} TilesType;

typedef enum {
    EMPTY_BLOCK_V = 0,
    BRICK_BLOCK_V,
    HARD_BRICK_BLOCK_V,
    PIPE_BLOCK_V,
    PIPE_TOP_BLOCK_V,
    COIN_BLOCK_V,
    STAR_BLOCK_V,
    MUSHROOM_BLOCK_V,
    FLOWER_BLOCK_V
} TilesValue;


struct tile {
    int x;
    int y;
    int w;
    int h;
    int active;
    int type;
    int highlight;
};

struct tile** load_level(char* levelPath);

struct entity** loadEntities(char* levelPath, int* n);

// Aponta para a tile presente naquele par de coordenadas
struct tile* pointToTile(int x, int y, struct tile** tiles);

// Checa se há colisão acima da entidade
int tileUpCollision(struct entity* en, struct tile** tiles);

// Checa se há colisão abaixo da entidade
int tileDownCollision(struct entity* en, struct tile** tiles);

// Checa se há colisão à esquerda da entidade
int tileLeftCollision(struct entity* en, struct tile** tiles);

// Checa se há colisao à direita da entidade
int tileRightCollision(struct entity* en, struct tile** tiles);

// Converte o valor C para um indice de um sprite
int getTileV(char c);

#endif