#ifndef LEVEL_INCLUDED
#define LEVEL_INCLUDED

#define MAP_WIDTH 234 // Quantas tiles tem na horizontal
#define MAP_HEIGHT 16 // Quantas tiles tem na vertical
#define TILE_WIDTH 40 // Largura de cada tile
#define TILE_HEIGHT 40 // Altura de cada tile
#define VIRTUAL_WIDTH (25 * TILE_WIDTH) // Tamanho horizontal da tela virtual
#define VIRTUAL_HEIGHT (MAP_HEIGHT * TILE_HEIGHT) // Tamanho vertical da tela virtual

typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;
struct entity;
struct entityList;

#define TILES_N 10 // Quantas tiles diferentes existem
typedef enum {
    EMPTY_BLOCK = ' ',
    BRICK_BLOCK = 'b',
    BRICK_BODY_BLOCK = 'i',
    HARD_BRICK_BLOCK = 'h',
    PIPE_BLOCK = 'p',
    PIPE_BODY_BLOCK = 'P',
    COIN_BLOCK = 'c',
    STAR_BLOCK = 's',
    MUSHROOM_BLOCK = 'm',
    FLOWER_BLOCK = 'f'
} TilesType;

struct tile {
    int x; // Posicação X
    int y; // Posicao Y
    int w; // Largura
    int h; // Altura 
    int active; // Se é um bloco ativo
    int type; // Tipo do bloco
    int content; // Quantos contéudos o bloco armazena, ex: 3 moedas
};

// Lê o arquivo de configuração do mapa, retorna as tiles dele e preenche as entidades
struct tile** loadLevel(char* levelPath, struct entityList* l, ALLEGRO_BITMAP*** sprites);

// Aponta para a tile presente naquele par de coordenadas
struct tile* pointToTile(int x, int y, struct tile** tiles);

// Retorna o ID da entidade correspondente conteúdo do bloco : -1 se não houver nada associado a ele
int specialTileContent(char type);

// Aponta para a tile em que há colisao acima, do contrário retorna NULL
struct tile* tileUpCollision(struct entity* en, struct tile** tiles);

// Aponta para a tile em que há colisao abaixo, do contrário retorna NULL
struct tile* tileDownCollision(struct entity* en, struct tile** tiles);

// Aponta para a tile em que há colisao a esquerda, do contrário retorna NULL
struct tile* tileLeftCollision(struct entity* en, struct tile** tiles);

// Aponta para a tile em que há colisao a direita, do contrário retorna NULL
struct tile* tileRightCollision(struct entity* en, struct tile** tiles);

// Verifica se esta no limite horizontal do mapa
int withinMapWidth(struct entity* en);

// Verifica se esta no limite vertical do mapa
int withinMapHeight(struct entity* en);

#endif