#ifndef ANIMATION_INCLUDED
#define ANIMATION_INCLUDED

#include "game.h"
#include "entity.h"
#include "level.h"
#include "entity_list.h"

#define RGB_MAX 255 // Valor máximo de um RGB

#define FRAMES_N 5 // Quantos frames tem uma sprite
#define IDLE_FRAME 0 // Indice padrão do frame de IDLE
#define JUMP_FRAME 1 // Indice padrão do frame de JUMPING
#define WALK_DURATION 10 // Duração padrão de cada frame da animação
#define WALK_START 2 // Indice do primeiro frame de walk
#define WALK_END 4 // Indice do ultimo frame de walk

#define ENTITY_SPRITES_N 12 // Quantas sprites de entidade tem ao total
typedef enum {
    CHAR_SPRITE = 0,
    SMALL_CHAR_SPRITE,
    CHAR_FLOWER_SPRITE,
    GOOMBA_SPRITE,
    TURTLE_SPRITE,
    FLOWER_SPRITE,
    STAR_SPRITE,
    SHELL_SPRITE,
    MUSHROOM_SPRITE,
    COIN_SPRITE,
    FIREBALL_SPRITE,
    CHECKPOINT_SPRITE
} EntitySpritesID;

#define TILE_SPRITES_N 10 // Quantas sprites de tile tem ao total
typedef enum {
    EMPTY_BLOCK_SPRITE = 0,
    BRICK_BLOCK_SPRITE,
    BRICK_BODY_SPRITE,
    HARD_BRICK_BLOCK_SPRITE,
    PIPE_BLOCK_SPRITE,
    PIPE_BODY_SPRITE,
    COIN_BLOCK_SPRITE,
    STAR_BLOCK_SPRITE,
    MUSHROOM_BLOCK_SPRITE,
    FLOWER_BLOCK_SPRITE
} TileSpritesID;

#define SCREENS_N 4 // Quantas telas tem ao total
typedef enum {
    START_SCREEN = 0,
    HELP_SCREEN,
    SCORES_SCREEN,
    BACKGROUND_SCREEN
} Screens;

#define END_SPRITES_N 3 // Quantas sprites tem a animação do fim

struct animation {
    int sprite; // ID da sprite que deve utilizar
    int duration; // Quantos clock duram cada frame
    int currentClock; // Clock atual
    int currentFrame; // Frame atual
    int reset; // Se deve começar a resetar os frames
    int start; // A primeira posição da animação na sprite
    int end; // A ultima posição da animação na sprite
};

// Carrega as sprites das tiles
ALLEGRO_BITMAP** loadTileSprites();

// Carrega as sprites das entidades
ALLEGRO_BITMAP*** loadEntitySprites();

// Carrega as telas
ALLEGRO_BITMAP** loadScreens();

// Retorna o ID da sprite correspondente ao tipo da entidade
int entitySpriteID(char type);

// Retorna o ID da sprite correspondente ao tipo da tile
int tileSpriteID(char type);

// Cria um conjunto de atributos utilizados para fazer uma animação
struct animation* newAnimation(int whichSprite, int start, int end, int duration);

// Desenha uma entidade qualquer utilizando um deslocamento de câmera e uma cor
void drawEntity(struct entity* en, int* offset, ALLEGRO_BITMAP*** sprites, ALLEGRO_COLOR color);

// Desenha todas as entidades de uma lista de entidades
void drawEntities(struct entityList* list, int* offset, ALLEGRO_BITMAP*** sprites, ALLEGRO_COLOR color);

// Desenha as tiles
void drawTiles(struct tile** tiles, ALLEGRO_BITMAP** sprites, int* offset);

// Desenha a tela de scores especificamente
int drawEnd(ALLEGRO_BITMAP** screens, ALLEGRO_SAMPLE** samples, ALLEGRO_AUDIO_STREAM** tracks, int* score);

// Desenha os menus e as telas do jogo
int drawScreen(ALLEGRO_BITMAP** screens, int which, ALLEGRO_SAMPLE** samples, ALLEGRO_AUDIO_STREAM** tracks);

#endif