#include "level.h"
#include "entity.h"
#include "utils.h"
#include "animation.h"
#include "entity_list.h"

int main()
{
    game_init();

    ALLEGRO_BITMAP*** sprites = loadSprites();
    ALLEGRO_BITMAP** tileSprites = loadTileSprites();

    ALLEGRO_SAMPLE* rojao = al_load_sample("resources/rojao.wav");
    must_init(rojao, "rojao");

    ALLEGRO_AUDIO_STREAM* soundtrack = al_load_audio_stream("resources/soundtrack.opus", 2, 2048);
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
    bool redraw = true;
    ALLEGRO_EVENT event;
    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));
    
    al_start_timer(timer);
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
                        removeEntity(next->id, entities);
                        character->dy = -12.5;
                        character->behavior = JUMPING;
                        al_play_sample(rojao, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    } else if(entityCollision(character, next->en))
                        done = true;
                    next = next->next;
                }
                
                offset = -(character->x - (DISPLAY_WIDTH / 2));
               
                if(key[ALLEGRO_KEY_ESCAPE]) done = true;
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

    al_destroy_display(disp);
    al_destroy_audio_stream(soundtrack);
    al_destroy_sample(rojao);
    al_destroy_font(font);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    for(int i = 0; i < TILES_N; i++) al_destroy_bitmap(tileSprites[i]);
    free(tileSprites);
    free(tiles[0]);
    free(tiles);
    destroyEntity(character);
    destroyList(entities);
    
    for(int i = 0; i < ENTITY_SPRITES_N; i++)
        for(int j = 0; j < FRAMES_N; j++)
            al_destroy_bitmap(sprites[i][j]);

    return 0;
}