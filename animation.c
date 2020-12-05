#include "animation.h"
#include "entity.h"
#include "utils.h"
#include "level.h"

#include <math.h>

ALLEGRO_BITMAP** loadTileSprites(){
    ALLEGRO_BITMAP** sprites = calloc(8, sizeof(ALLEGRO_BITMAP*));
    mustAllocate(sprites, "sprites");

    sprites[0] = al_load_bitmap("resources/brick.png");
    mustAllocate(sprites[0],"a");
    sprites[1] = al_load_bitmap("resources/brick.png");
    mustAllocate(sprites[1],"a");
    sprites[2] = al_load_bitmap("resources/brick.png");
    mustAllocate(sprites[2],"a");
    sprites[3] = al_load_bitmap("resources/pipe.png");
    mustAllocate(sprites[3],"a");
    sprites[4] = al_load_bitmap("resources/pipe_top.png");
    mustAllocate(sprites[4],"a");
    sprites[5] = al_load_bitmap("resources/surprise.png");
    mustAllocate(sprites[5],"a");
    sprites[6] = al_load_bitmap("resources/surprise.png");
    mustAllocate(sprites[6],"a");
    sprites[7] = al_load_bitmap("resources/surprise.png");
    mustAllocate(sprites[7],"a");
    sprites[8] = al_load_bitmap("resources/surprise.png");
    mustAllocate(sprites[8],"a");

    return sprites;
}

// Carrega as sprites do main character
ALLEGRO_BITMAP** loadMainFrames(){
    ALLEGRO_BITMAP** frames = malloc(sizeof(ALLEGRO_BITMAP*) * 4);
    mustAllocate(frames, "frames");
    frames[0] = al_load_bitmap("resources/sprites/mario_walk1.png");
    frames[1] = al_load_bitmap("resources/sprites/mario_idle.png");
    frames[2] = al_load_bitmap("resources/sprites/mario_walk2.png");
    frames[3] = al_load_bitmap("resources/sprites/mario_jump.png");
    return frames;
}

struct animation* newAnimation(ALLEGRO_BITMAP** frames, int lFrame, int fDuration){
    struct animation* a = calloc(1, sizeof(struct animation));
    mustAllocate(a, "animation");

    a->frames = frames;
    a->lastFrame = lFrame;
    a->frameDuration = fDuration;
    a->currentClock = fDuration;
    a->currentFrame = IDLE_FRAME;
    a->reset = 0;

    return a;
}

void updateWalkFrame(struct entity* en){
    if(en->anim->currentClock == 0){
        if(en->anim->currentFrame >= en->anim->lastFrame) en->anim->reset = 1;
        if(en->anim->currentFrame == 0) en->anim->reset = 0;
        
        if(!en->anim->reset) en->anim->currentFrame += 1;
        else en->anim->currentFrame -= 1;

        en->anim->currentClock = en->anim->frameDuration;
    } else en->anim->currentClock -= 1;
}


void drawEntity(struct entity* en, int* offset){
    switch(en->behavior){
        case IDLE:
            if(en->dir)
                al_draw_bitmap(en->anim->frames[1],
                floor(*offset + en->x), floor(en->y),
                0);
            else
                al_draw_scaled_bitmap(en->anim->frames[IDLE_FRAME],
                0, 0,
                en->w, en->h,
                floor(*offset + en->x) + en->w, floor(en->y),
                en->w * -1, en->h,
                0);
            break;
        case WALKING:
            updateWalkFrame(en);
            if(en->dir)
                al_draw_bitmap(en->anim->frames[en->anim->currentFrame],
                floor(*offset + en->x), floor(en->y),
                0);
            else
                al_draw_scaled_bitmap(en->anim->frames[en->anim->currentFrame],
                0, 0,
                en->w, en->h,
                floor(*offset + en->x) + en->w, floor(en->y),
                en->w * -1, en->h,
                0);
            break;
        case JUMPING:
            if(en->dir)
                al_draw_bitmap(en->anim->frames[JUMP_FRAME],
                floor(*offset + en->x), floor(en->y),
                0);
            else
                al_draw_scaled_bitmap(en->anim->frames[1],
                0, 0,
                en->w, en->h,
                floor(*offset + en->x) + en->w, floor(en->y),
                en->w * -1, en->h,
                0);
            break;
    }
}

void drawTiles(struct tile** tiles, ALLEGRO_BITMAP** sprites, int* offset){
    int whichSprite = 0;
    for(int y = 0; y < MAP_HEIGHT; y++){
        for(int x = 0; x < MAP_WIDTH; x++){
            if(tiles[y][x].active){
                whichSprite = getTileV(tiles[y][x].type);
                al_draw_bitmap(sprites[whichSprite],
                tiles[y][x].x + *offset,
                tiles[y][x].y,
                0);
            }
                
        }
    }
}