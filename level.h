#ifndef LEVEL_INCLUDED
#define LEVEL_INCLUDED

typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;
struct entity;
struct entityList;

#define TILES_N 9
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

struct tile {
    int x;
    int y;
    int w;
    int h;
    int active;
    int type;
    int highlight;
};

// Lê o arquivo de configuração do level, retorna as tiles dele e preenche as entidades
struct tile** loadLevel(char* levelPath, struct entityList* l, ALLEGRO_BITMAP*** sprites);

// Aponta para a tile presente naquele par de coordenadas
struct tile* pointToTile(int x, int y, struct tile** tiles);

// Retorna o ID da entidade correspondente ao tipo do bloco : -1 se não houver nada associado a ele
int specialTileContent(char type);

// Aponta para a tile em que há colisao acima, do contrário retorna NULL
struct tile* tileUpCollision(struct entity* en, struct tile** tiles);

// Aponta para a tile em que há colisao abaixo, do contrário retorna NULL
struct tile* tileDownCollision(struct entity* en, struct tile** tiles);

// Aponta para a tile em que há colisao a esquerda, do contrário retorna NULL
struct tile* tileLeftCollision(struct entity* en, struct tile** tiles);

// Aponta para a tile em que há colisao a direita, do contrário retorna NULL
struct tile* tileRightCollision(struct entity* en, struct tile** tiles);

// Converte o valor C para um indice de um sprite
int getTileV(char c);

#endif