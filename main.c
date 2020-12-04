#include "level.h"
#include "entity.h"
#include "utils.h"
#include "animation.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define IDLE 0
#define WALKING 1
#define JUMPING 2

int main()
{
    game_init();
    
    ALLEGRO_BITMAP* brick = al_load_bitmap("resources/brick.png");
    must_init(brick, "brick");

    ALLEGRO_BITMAP* surprise = al_load_bitmap("resources/surprise.png");
    must_init(surprise, "surprise");

    ALLEGRO_BITMAP* pipe = al_load_bitmap("resources/pipe.png");
    must_init(pipe, "pipe");

    ALLEGRO_BITMAP* pipe_top = al_load_bitmap("resources/pipe_top.png");
    must_init(pipe_top, "pipe_top");

    struct tile** tiles = load_level("level.txt");

    ALLEGRO_BITMAP** tileSprites = loadTileSprites();

    ALLEGRO_BITMAP** frames = malloc(sizeof(ALLEGRO_BITMAP*) * 4);
    frames[0] = al_load_bitmap("resources/sprites/mario_walk1.png");
    frames[1] = al_load_bitmap("resources/sprites/mario_idle.png");
    frames[2] = al_load_bitmap("resources/sprites/mario_walk2.png");
    frames[3] = al_load_bitmap("resources/sprites/mario_jump.png");

    struct animation* anim = newAnimation(frames, 2, FRAME_DURATION);
    struct entity* character = newEntity(120, 510, RIGHT, JUMPING, anim);

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
                updateCharacter(character, tiles, key);

                character->y += character->dy;
                character->x += character->dx;
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

            drawEntity(character, offset);
            drawTiles(tiles, tileSprites, &offset);

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