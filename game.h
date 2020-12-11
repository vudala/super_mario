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
    INIT = 0,
    PLAY,
    ENDING,
    DESTROY
} GameStates;

void gameInit();

int gamePlay(int* score, ALLEGRO_BITMAP** screens, ALLEGRO_SAMPLE** samples);

void gameDestroy();

#endif