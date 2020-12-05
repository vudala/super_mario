#include "level.h"
#include "entity.h"
#include "utils.h"
#include "animation.h"

int main()
{
    game_init();

    struct tile** tiles = load_level("level.txt");

    ALLEGRO_BITMAP** tileSprites = loadTileSprites();

    struct animation* anim = newAnimation(loadMainFrames(), 2, FRAME_DURATION);
    struct entity* character = newEntity(120, 510, RIGHT, JUMPING, anim);

    int entN = 0;
    struct entity** entities = loadEntities("level.txt", &entN);

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
                for(int i = 0; i < entN; i++) updateEntity(entities[i], tiles);
                
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
            al_clear_to_color(al_map_rgb(120, 60, 180));

            drawEntity(character, &offset);
            drawTiles(tiles, tileSprites, &offset);
            for(int i = 0; i < entN; i++) drawEntity(entities[i], &offset);
            char* aux = calloc(1, 50);
            sprintf(aux, "%d", character->behavior);
            al_draw_text(font, al_map_rgb(255, 0, 0), 20, 20, 0, aux);
            
            al_flip_display();
            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}