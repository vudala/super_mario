#ifndef SOUND_INCLUDED
#define SOUND_INCLUDED

#include "game.h"

#define SAMPLES_N 8
typedef enum {
    JUMP_SAMPLE = 0,
    BOUNCE_SAMPLE,
    COIN_SAMPLE,
    POWERUP_SAMPLE,
    FIREBALL_SAMPLE,
    SELECT_SAMPLE,
    UNPOWER_SAMPLE,
    HIT_SAMPLE
} SamplesIndex;

ALLEGRO_SAMPLE** loadSamples();
ALLEGRO_AUDIO_STREAM** loadStreams();

#endif