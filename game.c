#include "game.h"
#include "level.h"
#include "entity.h"
#include "animation.h"
#include "entity_list.h"

#include <stdio.h>

void must_init(bool test, const char *description){
    if(test) return;

    printf("Couldn't initialize %s\n", description);
    exit(1);
}

int gameInit(){
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_init_image_addon(), "image addon");
    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "audio codecs");
    must_init(al_reserve_samples(16), "reserve samples");

    timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    queue = al_create_event_queue();
    must_init(queue, "queue");

    disp = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    must_init(disp, "display");

    font = al_create_builtin_font();
    must_init(font, "font");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    al_start_timer(timer);

    memset(key, 0, sizeof(key));

    return PLAY;
}

int gamePlay(int* score){

    ALLEGRO_BITMAP*** sprites = loadSprites();
    ALLEGRO_BITMAP** tileSprites = loadTileSprites();

    ALLEGRO_SAMPLE* rojao = al_load_sample("resources/sounds/rojao.wav");
    must_init(rojao, "rojao");

    ALLEGRO_AUDIO_STREAM* soundtrack = al_load_audio_stream("resources/sounds/soundtrack.opus", 2, 2048);
    must_init(soundtrack, "soundtrack");
    al_set_audio_stream_playmode(soundtrack, ALLEGRO_PLAYMODE_LOOP);
    //al_attach_audio_stream_to_mixer(soundtrack, al_get_default_mixer());

    struct entityList* entities = malloc(sizeof(struct entityList));
    must_init(entities, "entities");
    createList(entities);

    struct tile** tiles = loadLevel("level.txt", entities, sprites);
    
    struct animation* anim = newAnimation(MAIN_CHARACTER_SPRITE, 2, FRAME_DURATION);
    struct entity* character = newEntity(MAIN_CHARACTER_SPRITE, 120, 510,
        al_get_bitmap_width(sprites[MAIN_CHARACTER_SPRITE][0]),
        al_get_bitmap_height(sprites[MAIN_CHARACTER_SPRITE][0]),
        RIGHT, JUMPING, anim);

    int offset = 0;

    bool done = false;
    int newState = DESTROY;
    bool redraw = true;
    
    for(;;)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                updateCharacter(character, tiles, entities, key);

                struct entityNode* next = entities->start;
                while(next != NULL){
                    updateEntity(next->en, tiles);
                    // Se pulou em cima de um inimigo
                    if(entityDownCollision(character, next->en)){
                        *score += 100;
                        removeEntity(next->id, entities);
                        character->dy = -12.5;
                        character->behavior = JUMPING;
                        al_play_sample(rojao, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    } else if(entityCollision(character, next->en)){
                        newState = ENDING;
                        done = true;
                    }

                        
                    next = next->next;
                }
                
                offset = -(character->x - (DISPLAY_WIDTH / 2));
               
                if(key[ALLEGRO_KEY_ESCAPE]) {
                    newState = DESTROY;
                    done = true;
                }

                for(int i = 0; i < ALLEGRO_KEY_MAX; i++) key[i] &= KEY_SEEN;

                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
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

        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(127, 127, 127));
            // Desenha o mapa
            drawTiles(tiles, tileSprites, &offset);
            // Desenha o personagem principal
            drawEntity(character, &offset, sprites);
            // Desenha todas as outras entidades
            struct entityNode* next = entities->start;
            while(next != NULL){
                drawEntity(next->en, &offset, sprites);
                next = next->next;
            }
            al_flip_display();
            redraw = false;
        }
    }

    al_destroy_audio_stream(soundtrack);
    al_destroy_sample(rojao);
    
    for(int i = 0; i < TILES_N; i++) al_destroy_bitmap(tileSprites[i]);
    free(tileSprites);
    free(tiles[0]);
    free(tiles);
    destroyEntity(character);
    destroyList(entities);
    
    for(int i = 0; i < ENTITY_SPRITES_N; i++)
        for(int j = 0; j < FRAMES_N; j++)
            al_destroy_bitmap(sprites[i][j]);

    return newState;
}

int gameEnding(int* score){
    bool done = false;
    int newState = DESTROY;
    bool redraw = true;
    
    for(;;)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
               
                if(key[ALLEGRO_KEY_ESCAPE]) {
                    newState = DESTROY;
                    done = true;
                }

                if(key[ALLEGRO_KEY_ENTER]) {
                    newState = PLAY;
                    *score = 0;
                    done = true;
                }
                
                for(int i = 0; i < ALLEGRO_KEY_MAX; i++) key[i] &= KEY_SEEN;

                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
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

        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(127, 127, 127));
            char* aux = malloc(50);
            sprintf(aux, "%d", *score);
            al_draw_text(font, al_map_rgb(255, 0, 0), 20, 20, 0, aux);
            al_flip_display();
            redraw = false;
        }
    }

    return newState;
}

int gameDestroy(){
    al_destroy_display(disp);
    al_destroy_font(font);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    exit(0);
}