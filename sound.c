// ALLEGRO_AUDIO_STREAM* soundtrack = al_load_audio_stream("resources/sounds/soundtrack.opus", 2, 2048);
// mustInit(soundtrack, "soundtrack");
// al_set_audio_stream_playmode(soundtrack, ALLEGRO_PLAYMODE_LOOP);
// al_attach_audio_stream_to_mixer(soundtrack, al_get_default_mixer());

// ALLEGRO_SAMPLE* rojao = al_load_sample("resources/sounds/rojao.wav");
//     mustInit(rojao, "rojao");

#include "sound.h"
#include "utils.h"

ALLEGRO_SAMPLE** loadSamples(){
    ALLEGRO_SAMPLE** samples = malloc(sizeof(ALLEGRO_SAMPLE*) * SAMPLES_N);
    mustAllocate(samples, "samples");

    samples[JUMP_SAMPLE] = al_load_sample("resources/sounds/jump.wav");
    samples[BOUNCE_SAMPLE] = al_load_sample("resources/sounds/bounce.wav");
    samples[COIN_SAMPLE] = al_load_sample("resources/sounds/pickupCoin.wav");
    samples[POWERUP_SAMPLE] = al_load_sample("resources/sounds/powerUp.wav");
    samples[FIREBALL_SAMPLE] = al_load_sample("resources/sounds/fireball.wav");
    samples[SELECT_SAMPLE] = al_load_sample("resources/sounds/select.wav");
    samples[UNPOWER_SAMPLE] = al_load_sample("resources/sounds/unpower.wav");
    samples[HIT_SAMPLE] = al_load_sample("resources/sounds/hit.wav");

    return samples;
}