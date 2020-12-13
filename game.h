#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#define FPS 60.0

#define KEY_SEEN     1
#define KEY_RELEASED 2

ALLEGRO_TIMER* timer;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_DISPLAY* disp;
ALLEGRO_FONT* font;
ALLEGRO_EVENT event;
unsigned char key[ALLEGRO_KEY_MAX];

typedef enum {
    PLAY = 0,
    DESTROY
} GameStates;

// Inicia as estruturas do Allegro
void gameInit();

// Joga o jogo
int gamePlay(int* score, ALLEGRO_BITMAP** screens, ALLEGRO_SAMPLE** samples, ALLEGRO_AUDIO_STREAM** tracks);

// Destrói as estruturas do Allegro
void gameDestroy(ALLEGRO_BITMAP** screens, ALLEGRO_SAMPLE** samples, ALLEGRO_AUDIO_STREAM** tracks);

#endif