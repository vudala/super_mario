#include "sound.h"
#include "utils.h"

ALLEGRO_SAMPLE** loadSamples(){
    ALLEGRO_SAMPLE** samples = malloc(sizeof(ALLEGRO_SAMPLE*) * SAMPLES_N);
    mustAllocate(samples, "samples");

    samples[JUMP_SAMPLE] = al_load_sample("resources/sounds/samples/jump.wav");
    mustInit(samples[JUMP_SAMPLE], "JUMP_SAMPLE");

    samples[BOUNCE_SAMPLE] = al_load_sample("resources/sounds/samples/bounce.wav");
    mustInit(samples[BOUNCE_SAMPLE], "BOUNCE_SAMPLE");

    samples[COIN_SAMPLE] = al_load_sample("resources/sounds/samples/pickupCoin.wav");
    mustInit(samples[COIN_SAMPLE], "COIN_SAMPLE");

    samples[POWERUP_SAMPLE] = al_load_sample("resources/sounds/samples/powerUp.wav");
    mustInit(samples[POWERUP_SAMPLE], "POWERUP_SAMPLE");

    samples[FIREBALL_SAMPLE] = al_load_sample("resources/sounds/samples/fireball.wav");
    mustInit(samples[FIREBALL_SAMPLE], "FIREBALL_SAMPLE");

    samples[SELECT_SAMPLE] = al_load_sample("resources/sounds/samples/select.wav");
    mustInit(samples[SELECT_SAMPLE], "SELECT_SAMPLE");

    samples[UNPOWER_SAMPLE] = al_load_sample("resources/sounds/samples/unpower.wav");
    mustInit(samples[UNPOWER_SAMPLE], "UNPOWER_SAMPLE");

    samples[HIT_SAMPLE] = al_load_sample("resources/sounds/samples/hit.wav");
    mustInit(samples[HIT_SAMPLE], "HIT_SAMPLE");

    samples[DEATH_SAMPLE] = al_load_sample("resources/sounds/samples/death.wav");
    mustInit(samples[DEATH_SAMPLE], "DEATH_SAMPLE");

    samples[CHECKPOINT_SAMPLE] = al_load_sample("resources/sounds/samples/checkpoint.wav");
    mustInit(samples[CHECKPOINT_SAMPLE], "CHECKPOINT_SAMPLE");

    samples[SECRET_SAMPLE] = al_load_sample("resources/sounds/samples/secret.wav");
    mustInit(samples[SECRET_SAMPLE], "SECRET_SAMPLE");

    return samples;
}

ALLEGRO_AUDIO_STREAM** loadTracks(){
    ALLEGRO_AUDIO_STREAM** tracks = malloc(sizeof(ALLEGRO_AUDIO_STREAM*) * TRACKS_N);
    mustInit(tracks, "tracks");

    tracks[GAME_TRACK] = al_load_audio_stream("resources/sounds/tracks/game_track.opus", 2, 2048);
    mustInit(tracks[GAME_TRACK], "GAME_TRACK");
    al_set_audio_stream_playmode(tracks[GAME_TRACK], ALLEGRO_PLAYMODE_LOOP);

    tracks[END_TRACK] = al_load_audio_stream("resources/sounds/tracks/end_track.opus", 2, 2048);
    mustInit(tracks[END_TRACK], "END_TRACK");
    al_set_audio_stream_playmode(tracks[END_TRACK], ALLEGRO_PLAYMODE_LOOP);

    tracks[STAR_TRACK] = al_load_audio_stream("resources/sounds/tracks/star_track.opus", 2, 2048);
    mustInit(tracks[STAR_TRACK], "STAR_TRACK");
    al_set_audio_stream_playmode(tracks[STAR_TRACK], ALLEGRO_PLAYMODE_LOOP);

    return tracks;
}