#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#define FPS 60.0
#define MAP_WIDTH 337 // Quantas tiles tem na horizontal
#define MAP_HEIGHT 16 // Quantas tiles tem na vertical
#define TILE_WIDTH 40 // Largura de cada tile
#define TILE_HEIGHT 40 // Altura de cada tile
#define DISPLAY_WIDTH 1024 // Tamanho horizontal da tela
#define DISPLAY_HEIGHT (MAP_HEIGHT * TILE_HEIGHT) // Tamanho vertical da tela

#define TOP_SCORE_N 10 // Quantos scores são guardados no top score

#define KEY_SEEN     1
#define KEY_RELEASED 2

ALLEGRO_TIMER* timer;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_DISPLAY* disp;
ALLEGRO_FONT* font;
ALLEGRO_EVENT event;
unsigned char key[ALLEGRO_KEY_MAX];

typedef enum {
    INIT = 0,
    PLAY,
    ENDING,
    DESTROY
} GameStates;

int gameInit();
int gamePlay(int* score);
int gameEnding(int* score);
int gameDestroy();

#endif