// GRR20195689 Eduardo Vudala Senoski

#include "game.h"
#include "level.h"
#include "entity.h"
#include "animation.h"
#include "entity_list.h"
#include "utils.h"
#include "character.h"
#include "sound.h"

#include <stdio.h>

void gameInit(){
    // Inicia as estruturas principais do Allegro
    mustInit(al_init(), "allegro");
    mustInit(al_install_keyboard(), "keyboard");
    mustInit(al_init_image_addon(), "image addon");
    mustInit(al_install_audio(), "audio addon");
    mustInit(al_init_acodec_addon(), "audio codecs addon");
    mustInit(al_reserve_samples(16), "reserve samples addon");
    mustInit(al_init_font_addon(), "font addon");
    mustInit(al_init_ttf_addon(), "font addon");

    srand((unsigned) time(NULL));

    timer = al_create_timer(1.0 / FPS);
    mustInit(timer, "timer");

    queue = al_create_event_queue();
    mustInit(queue, "queue");

    disp = al_create_display(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    mustInit(disp, "display");

    font = al_load_font("resources/database/Pixellari.ttf", 30, 0);
    mustInit(font, "font");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    al_start_timer(timer);

    memset(key, 0, sizeof(key));
}

void updateCameraOffset(int* offset, struct character* character){
    // Desloca a camera relativa ao personagem
    *offset = -(character->self->x - (VIRTUAL_WIDTH / 2));
    // Se bater na borda esquerda desloca um valor constante
    if(*offset > -TILE_WIDTH)
        *offset = -TILE_WIDTH;
     // Se bater na borda direita desloca um valor constante para escondê-la
    else if (character->self->x + (VIRTUAL_WIDTH / 2) > (TILE_WIDTH) * (MAP_WIDTH - 2))
        *offset = -(TILE_WIDTH * (MAP_WIDTH - 2) - VIRTUAL_WIDTH);
}

int gamePlay(int* score, ALLEGRO_BITMAP** screens, ALLEGRO_SAMPLE** samples, ALLEGRO_AUDIO_STREAM** tracks){
    // Carrega as sprites
    ALLEGRO_BITMAP*** entitySprites = loadEntitySprites();
    ALLEGRO_BITMAP** tileSprite = loadTileSprites();

    // Cria a lista de entidades
    struct entityList* entities = newList();

    // Cria a lista de fireballs
    struct entityList* fireballs = newList();

    // Carrega o mapa
    struct tile** tiles = loadLevel("resources/database/level.txt", entities, entitySprites);
    
    // Inicia o personagem
    struct character* character = newCharacter(
        newEntity(
            MAIN_SMALL, 120, 510,
            SMALL_WIDTH, SMALL_HEIGHT, RIGHT, 
            newAnimation(SMALL_CHAR_SPRITE, WALK_START, WALK_END, WALK_DURATION),
            INFINITE
        )
    );

    al_rewind_audio_stream(tracks[GAME_TRACK]); // Reseta a track  
    al_attach_audio_stream_to_mixer(tracks[GAME_TRACK], al_get_default_mixer()); // Começa a track

    // Para imprimir a pontuação
    char* aux = malloc(9 * sizeof(char));
    mustAllocate(aux, "aux string");

    bool done = false;
    bool redraw = true;
    int newState = DESTROY;
    struct tile* t = NULL;

    int offset = 0;
    struct entity* newEn = NULL;
    
    for(;;){
        al_wait_for_event(queue, &event);
        switch(event.type){
            case ALLEGRO_EVENT_TIMER:
                t = updateCharacter(character, tiles, fireballs, samples, tracks);
                newEn = tileInteract(character, t, samples, score);
                if(newEn) insertEntity(entities, newEn);

                // Se colidiu com outra entidade inimiga sem matá-la, termina o jogo
                if(entitiesInteract(character, tiles, entities, fireballs, samples, tracks, score)){
                    al_detach_audio_stream(tracks[GAME_TRACK]); // Para a música
                    al_detach_audio_stream(tracks[STAR_TRACK]); // Para a música
                    sleep(2); // Espera um pouco
                    newState = drawEnd(screens, samples, tracks, score);
                    done = true;
                    break;
                }
                
                updateCameraOffset(&offset, character);
               
                if(key[ALLEGRO_KEY_ESCAPE]) {
                    newState = DESTROY;
                    done = true;
                    break;
                }

                for(int i = 0; i < ALLEGRO_KEY_MAX; i++) key[i] &= KEY_SEEN;

                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                if(key[ALLEGRO_KEY_H]){  // Se apertou H, desenha a tela de ajuda
                    al_set_mixer_playing(al_get_default_mixer(), false);
                    drawScreen(screens, HELP_SCREEN, samples, tracks);
                    al_set_mixer_playing(al_get_default_mixer(), true);
                }
                    
                break;
            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= KEY_RELEASED;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                newState = DESTROY;
                done = true;
                break;
        }

        if(done) break;

        if(redraw && al_is_event_queue_empty(queue)){
            // Desenha o mapa
            al_draw_bitmap(screens[BACKGROUND_SCREEN], 0, 0, 0);
            drawTiles(tiles, tileSprite, &offset);

            // Pintar de branco não interfere na cor
            ALLEGRO_COLOR color = al_map_rgb(255,255,255);

            // Desenha as diversas entidades
            drawEntities(fireballs, &offset, entitySprites, color);
            drawEntities(entities, &offset, entitySprites, color);

            // Desenha o personagem principal
            if(character->star) // Se estiver no modo estrela, desenha em cores aleatórias
                color = al_map_rgb(rand() % RGB_MAX, rand() % RGB_MAX, rand() % RGB_MAX);
                
            drawEntity(character->self, &offset, entitySprites, color);

            // Desenha o score no canto da tela em vermelho
            sprintf(aux, "%d", *score);
            al_draw_text(font, al_map_rgb(200, 60, 0), 20, 20, 0, aux);
            
            al_flip_display();
            redraw = false;
        }
    }

    // Destroi as estruturas
    for(int i = 0; i < TILES_N; i++) al_destroy_bitmap(tileSprite[i]);
    free(tileSprite);
    tileSprite = NULL;

    free(tiles[0]);
    free(tiles);
    tiles = NULL;

    destroyEntity(character->self);
    free(character);
    character = NULL;

    destroyList(entities);
    entities = NULL;

    destroyList(fireballs);
    fireballs = NULL;
    
    for(int i = 0; i < ENTITY_SPRITES_N; i++){
        for(int j = 0; j < FRAMES_N; j++)
            al_destroy_bitmap(entitySprites[i][j]);
        free(entitySprites[i]);
    }
    free(entitySprites);    
    entitySprites = NULL;

    free(aux);
    aux = NULL;
            
    *score = 0;

    return newState;
}

void gameDestroy(ALLEGRO_BITMAP** screens, ALLEGRO_SAMPLE** samples, ALLEGRO_AUDIO_STREAM** tracks){
    for(int i = 0; i < SAMPLES_N; i++)
        al_destroy_sample(samples[i]);
    free(samples);

    for(int i = 0; i < SCREENS_N; i++)
        al_destroy_bitmap(screens[i]);
    free(screens);

    for(int i = 0; i < TRACKS_N; i++)
        al_destroy_audio_stream(tracks[i]);
    free(tracks);
    
    al_destroy_display(disp);
    al_destroy_font(font);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
}