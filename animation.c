#include "animation.h"
#include "entity.h"
#include "utils.h"
#include "level.h"
#include "game.h"
#include "sound.h"
#include "stdio.h"

#include <math.h>

// Carrega uma sprite de n frames
ALLEGRO_BITMAP** loadSprite(char* path, int frames_n){
    ALLEGRO_BITMAP* mainBitmap = al_load_bitmap(path);
   
    int width = al_get_bitmap_width(mainBitmap);
    int height = al_get_bitmap_height(mainBitmap);
    int frameWidth = width / frames_n;

    ALLEGRO_BITMAP** sprites = calloc(frames_n, sizeof(ALLEGRO_BITMAP*));
    mustAllocate(sprites, "sprites");

    ALLEGRO_BITMAP* subBitmap = NULL;

    for(int i = 0; i < frames_n; i++){
        subBitmap = al_create_sub_bitmap(mainBitmap, i * frameWidth, 0, frameWidth, height);
        mustAllocate(subBitmap, path);
        sprites[i] = al_clone_bitmap(subBitmap);
    }

    al_destroy_bitmap(mainBitmap);
    
    return sprites;
}

ALLEGRO_BITMAP** loadScreens(){
    ALLEGRO_BITMAP** screens = calloc(SCREENS_N, sizeof(ALLEGRO_BITMAP*));
    mustAllocate(screens, "screens");

    screens[START_SCREEN] = al_load_bitmap("resources/sprites/screens/start.png");
    mustAllocate(screens[START_SCREEN], "start screen");

    screens[HELP_SCREEN] = al_load_bitmap("resources/sprites/screens/help.png");
    mustAllocate(screens[HELP_SCREEN],"help screen");
    
    screens[SCORES_SCREEN] = al_load_bitmap("resources/sprites/screens/scores.png");
    mustAllocate(screens[SCORES_SCREEN],"scores screen");

    return screens;
}

ALLEGRO_BITMAP** loadTileSprites(){
    ALLEGRO_BITMAP** sprites = calloc(TILE_SPRITES_N, sizeof(ALLEGRO_BITMAP*));
    mustAllocate(sprites, "sprites");

    sprites[0] = al_load_bitmap("resources/sprites/tiles/brick.png");
    mustAllocate(sprites[0],"a");
    sprites[1] = al_load_bitmap("resources/sprites/tiles/brick.png");
    mustAllocate(sprites[1],"a");
    sprites[2] = al_load_bitmap("resources/sprites/tiles/hard_brick.png");
    mustAllocate(sprites[2],"a");
    sprites[3] = al_load_bitmap("resources/sprites/tiles/pipe.png");
    mustAllocate(sprites[3],"a");
    sprites[4] = al_load_bitmap("resources/sprites/tiles/pipe_top.png");
    mustAllocate(sprites[4],"a");
    sprites[5] = al_load_bitmap("resources/sprites/tiles/surprise.png");
    mustAllocate(sprites[5],"a");
    sprites[6] = al_load_bitmap("resources/sprites/tiles/surprise.png");
    mustAllocate(sprites[6],"a");
    sprites[7] = al_load_bitmap("resources/sprites/tiles/surprise.png");
    mustAllocate(sprites[7],"a");
    sprites[8] = al_load_bitmap("resources/sprites/tiles/surprise.png");
    mustAllocate(sprites[8],"a");

    return sprites;
}

ALLEGRO_BITMAP*** loadSprites(){
    ALLEGRO_BITMAP*** sprites = calloc(ENTITY_SPRITES_N, sizeof(ALLEGRO_BITMAP**));
    mustAllocate(sprites, "sprites");

    sprites[CHAR_SPRITE] = loadSprite("resources/sprites/entities/mario.png" , FRAMES_N);
    sprites[SMALL_CHAR_SPRITE] = loadSprite("resources/sprites/entities/mario_small.png", FRAMES_N);
    sprites[CHAR_FLOWER_SPRITE] = loadSprite("resources/sprites/entities/mario_flower.png", FRAMES_N);
    sprites[GOOMBA_SPRITE] = loadSprite("resources/sprites/entities/goomba.png", FRAMES_N);
    sprites[TURTLE_SPRITE] = loadSprite("resources/sprites/entities/turtle.png", FRAMES_N);
    sprites[FLOWER_SPRITE] = loadSprite("resources/sprites/entities/flower.png", FRAMES_N);
    sprites[STAR_SPRITE] = loadSprite("resources/sprites/entities/star.png", FRAMES_N);
    sprites[SHELL_SPRITE] = loadSprite("resources/sprites/entities/shell.png", FRAMES_N);
    sprites[MUSHROOM_SPRITE] = loadSprite("resources/sprites/entities/mushroom.png", FRAMES_N);
    sprites[COIN_SPRITE] = loadSprite("resources/sprites/entities/coin.png", FRAMES_N);
    sprites[FIREBALL_SPRITE] = loadSprite("resources/sprites/entities/fireball.png", FRAMES_N);

    return sprites;
}
 
int entitySpriteID(char type){
    switch(type){
        case MAIN_CHARACTER: return CHAR_SPRITE; break;
        case MAIN_SMALL: return SMALL_CHAR_SPRITE; break;
        case MAIN_FLOWER: return CHAR_FLOWER_SPRITE; break;
        case GOOMBA: return GOOMBA_SPRITE; break;
        case TURTLE: return TURTLE_SPRITE; break;
        case FLOWER: return FLOWER_SPRITE; break;
        case STAR: return STAR_SPRITE; break;
        case SHELL: return SHELL_SPRITE; break;
        case MUSHROOM: return MUSHROOM_SPRITE; break;
        case COIN: return COIN_SPRITE; break;
        case FIREBALL: return FIREBALL_SPRITE; break;
    }

    return MUSHROOM_SPRITE;
}

struct animation* newAnimation(int sprite){
    struct animation* a = calloc(1, sizeof(struct animation));
    mustAllocate(a, "animation");

    a->sprite = sprite;
    a->currentClock = FRAME_DURATION;
    a->currentFrame = IDLE_FRAME;
    a->reset = 0;

    return a;
}

void updateWalkFrame(struct entity* en){
    if(en->anim->currentClock == 0){
        if(en->anim->currentFrame >= WALK_END) en->anim->reset = 1;
        if(en->anim->currentFrame == WALK_START) en->anim->reset = 0;
        
        if(!en->anim->reset) en->anim->currentFrame += 1;
        else en->anim->currentFrame -= 1;

        en->anim->currentClock = WALK_END;
    } else en->anim->currentClock -= 1;
}

void drawEntity(struct entity* en, int* offset, ALLEGRO_BITMAP*** sprites, ALLEGRO_COLOR color){
    // Se estiver virado pra direita desenha normalmente, se não irá desenhar ao contrário
    int xScale = en->dir ? 1 : -1;
    // Deslocamento do desenho
    int dw = en->dir ? 0 : en->w;

    // Seleciona qual frame deve desenhar
    ALLEGRO_BITMAP* frame = sprites[en->anim->sprite][IDLE_FRAME];
    switch(en->behavior){
        case WALKING:
            updateWalkFrame(en);
            frame = sprites[en->anim->sprite][en->anim->currentFrame];
            break;
        case JUMPING: case BOUNCING:
            frame = sprites[en->anim->sprite][JUMPING];
            break;
    }

    al_draw_tinted_scaled_bitmap(frame, color, 0, 0, en->w, en->h,
    floor(*offset + en->x) + dw, floor(en->y),
    en->w * xScale, en->h, 0);
}

void drawEntities(struct entityList* list, int* offset, ALLEGRO_BITMAP*** sprites, ALLEGRO_COLOR color){
    struct entityNode* next = list->start;
    while(next != NULL){
        drawEntity(next->en, offset, sprites, color);
        next = next->next;
    }
}

int tileSpriteID(char type){
    switch(type){
        case EMPTY_BLOCK: return EMPTY_BLOCK_SPRITE; break;
        case BRICK_BLOCK: return BRICK_BLOCK_SPRITE;
        case HARD_BRICK_BLOCK: return HARD_BRICK_BLOCK_SPRITE; break;
        case PIPE_BLOCK: return PIPE_BLOCK_SPRITE; break;
        case PIPE_TOP_BLOCK: return PIPE_TOP_BLOCK_SPRITE; break;
        case COIN_BLOCK: return COIN_BLOCK_SPRITE; break;
        case STAR_BLOCK: return STAR_BLOCK_SPRITE; break;
        case MUSHROOM_BLOCK: return MUSHROOM_BLOCK_SPRITE; break;
        case FLOWER_BLOCK: return FLOWER_BLOCK_SPRITE; break;
    }
    
    return EMPTY_BLOCK_SPRITE;
}

void drawTiles(struct tile** tiles, ALLEGRO_BITMAP** sprites, int* offset){
    int whichSprite = 0;
    for(int y = 0; y < MAP_HEIGHT; y++){
        for(int x = 0; x < MAP_WIDTH; x++){
            if(tiles[y][x].active){
                whichSprite = tileSpriteID(tiles[y][x].type);
                if(tiles[y][x].content){
                    al_draw_bitmap(sprites[whichSprite],
                    tiles[y][x].x + *offset,
                    tiles[y][x].y,
                    0);
                }
                else {
                    // Se não tiver conteudos no bloco, pinta de cinza
                    al_draw_tinted_bitmap(sprites[whichSprite],
                    al_map_rgb(105,105,105), 
                    tiles[y][x].x + *offset,
                    tiles[y][x].y,
                    0);
                }
            }
        }
    }
}

int drawScreen(ALLEGRO_BITMAP** screens, int which, ALLEGRO_SAMPLE** samples, int* score){
    bool done = false;
    bool redraw = true;
    int newState = DESTROY;
    
    for(;;)
    {
        al_wait_for_event(queue, &event);
        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                switch(event.keyboard.keycode){
                    case ALLEGRO_KEY_ESCAPE:
                        al_play_sample(samples[SELECT_SAMPLE], 1.0, 1.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        done = true;
                        break;
                    case ALLEGRO_KEY_H:
                        if(which == START_SCREEN){
                            al_play_sample(samples[SELECT_SAMPLE], 1.0, 1.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            drawScreen(screens, HELP_SCREEN, samples, NULL);
                        }
                        break;
                    case ALLEGRO_KEY_UP:
                        if(which == START_SCREEN){
                            al_play_sample(samples[SELECT_SAMPLE], 1.0, 1.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            newState = PLAY;
                            done = true;
                        }
                        break;
                    case ALLEGRO_KEY_ENTER:
                        if(which == SCORES_SCREEN){
                            al_play_sample(samples[SELECT_SAMPLE], 1.0, 1.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            newState = PLAY;
                            done = true;
                        }
                        break;
                }
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done) break;

        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(127, 127, 127));

            al_draw_bitmap(screens[which], 0, 0, 0);

            if(which == SCORES_SCREEN){
                int* scores = getScores(score);
                char* aux = malloc(sizeof(char) * 9); // Comporta um decimal de até 10 casas
                for(int i = 0; i < TOP_SCORE_N; i++){
                    sprintf(aux, "%2d %9d", i, scores[i]); 
                    al_draw_text(font, al_map_rgb(RGB_MAX, RGB_MAX, RGB_MAX), 600, 150 + i * 40, 0 , aux);
                }
                
            }
                 
            al_flip_display();
            redraw = false;
        }
    }
    
    return newState;
}