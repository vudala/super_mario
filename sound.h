// GRR20195689 Eduardo Vudala Senoski

#ifndef SOUND_INCLUDED
#define SOUND_INCLUDED

#include "game.h"

#define SAMPLES_N 11 // Quantas samples tem
typedef enum {
    JUMP_SAMPLE = 0,
    BOUNCE_SAMPLE,
    COIN_SAMPLE,
    POWERUP_SAMPLE,
    FIREBALL_SAMPLE,
    SELECT_SAMPLE,
    UNPOWER_SAMPLE,
    HIT_SAMPLE,
    DEATH_SAMPLE,
    CHECKPOINT_SAMPLE,
    SECRET_SAMPLE
} SamplesIndex;

#define TRACKS_N 3 // Quantas tracks tem
typedef enum {
    GAME_TRACK = 0,
    END_TRACK,
    STAR_TRACK
} TracksIndex;

// Carrega as samples
ALLEGRO_SAMPLE** loadSamples();

// Carrega as tracks
ALLEGRO_AUDIO_STREAM** loadTracks();

#endif