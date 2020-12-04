#ifndef ANIMATION_INCLUDED
#define ANIMATION_INCLUDED

#define FRAME_DURATION 10
#define IDLE_FRAME 1
#define JUMP_FRAME 3

#include "game.h"
struct entity;
struct tile;

struct animation {
    ALLEGRO_BITMAP** frames;
    int lastFrame; // Indice do último frame da animação
    int frameDuration; // Quantos clocks duram um frame
    int currentClock; // Clock atual
    int currentFrame; // Frame atual
    int reset; // Se deve começar a resetar os frames
};

struct ALLEGRO_BITMAP** loadTileSprites();

// Cria um conjunto de animções baseados em arquivos providos
struct animation* newAnimation(ALLEGRO_BITMAP** frames, int lFrame, int fDuration);

// Desenha uma entidade qualquer utilizando um deslocamento
void drawEntity(struct entity* en, int offset);

// Desenha as tiles
void drawTiles(struct tile** tiles, ALLEGRO_BITMAP** sprites, int* offset);

#endif