#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#define MAP_WIDTH 337
#define MAP_HEIGHT 16
#define TILE_WIDTH 40
#define TILE_HEIGHT 40
#define DISPLAY_WIDTH 1024
#define DISPLAY_HEIGHT (MAP_HEIGHT * TILE_HEIGHT)

#define KEY_SEEN     1
#define KEY_RELEASED 2

ALLEGRO_TIMER* timer;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_DISPLAY* disp;
ALLEGRO_FONT* font;

void game_init();
void must_init(bool test, const char *description);

#endif