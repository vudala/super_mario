#include "animation.h"
#include "entity.h"
#include "utils.h"
#include "level.h"
#include "game.h"

#include <math.h>

ALLEGRO_BITMAP** loadTileSprites(){
    ALLEGRO_BITMAP** sprites = calloc(TILES_SPRITES_N, sizeof(ALLEGRO_BITMAP*));
    mustAllocate(sprites, "sprites");

    sprites[0] = al_load_bitmap("resources/sprites/tiles/brick.png");
    mustAllocate(sprites[0],"a");
    sprites[1] = al_load_bitmap("resources/sprites/tiles/brick.png");
    mustAllocate(sprites[1],"a");
    sprites[2] = al_load_bitmap("resources/sprites/tiles/brick.png");
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

// Carrega as sprites do goomba
ALLEGRO_BITMAP** loadGoombaFrames(){
    ALLEGRO_BITMAP** frames = malloc(sizeof(ALLEGRO_BITMAP*) * 4);
    mustAllocate(frames, "goomba frames");

    frames[0] = al_load_bitmap("resources/sprites/goomba/goomba_walk1.png");
    mustAllocate(frames[0], "goomba_walk1");
    frames[1] = al_load_bitmap("resources/sprites/goomba/goomba_idle.png");
    mustAllocate(frames[1], "goombla_idle");
    frames[2] = al_load_bitmap("resources/sprites/goomba/goomba_walk2.png");
    mustAllocate(frames[2], "goomba_walk2");
    frames[3] = al_load_bitmap("resources/sprites/goomba/goomba_jump.png");
    mustAllocate(frames[3], "goomba_jump");

    return frames;
}

// Carrega as sprites do main character
ALLEGRO_BITMAP** loadMainFrames(){
    ALLEGRO_BITMAP** frames = malloc(sizeof(ALLEGRO_BITMAP*) * 4);
    mustAllocate(frames, "frames");

    frames[0] = al_load_bitmap("resources/sprites/mario/mario_walk1.png");
    frames[1] = al_load_bitmap("resources/sprites/mario/mario_idle.png");
    frames[2] = al_load_bitmap("resources/sprites/mario/mario_walk2.png");
    frames[3] = al_load_bitmap("resources/sprites/mario/mario_jump.png");
    
    return frames;
}

ALLEGRO_BITMAP*** loadSprites(){
    ALLEGRO_BITMAP*** sprites = calloc(ENTITY_SPRITES_N, sizeof(ALLEGRO_BITMAP**));
    must_init(sprites, "sprites");

    sprites[MAIN_CHARACTER_SPRITE] = loadMainFrames();
    sprites[GOOMBA_SPRITE] = loadGoombaFrames();
    sprites[TURTLE_SPRITE] = loadGoombaFrames();
    sprites[FLOWER_SPRITE] = loadGoombaFrames();
    sprites[STAR_SPRITE] = loadGoombaFrames();
    sprites[SHELL_SPRITE] = loadGoombaFrames();
    sprites[MUSHROOM_SPRITE] = loadGoombaFrames();

    return sprites;
}

int entitySpriteID(char type){
    switch(type){
        case MAIN_CHARACTER: return MAIN_CHARACTER_SPRITE; break;
        case GOOMBA: return GOOMBA_SPRITE; break;
        case TURTLE: return TURTLE_SPRITE; break;
        case FLOWER: return FLOWER_SPRITE; break;
        case STAR: return STAR_SPRITE; break;
        case SHELL: return SHELL_SPRITE; break;
        case MUSHROOM: return MUSHROOM_SPRITE; break;
    }
    return MUSHROOM_SPRITE;
}

struct animation* newAnimation(int whichSprite, int lFrame, int fDuration){
    struct animation* a = calloc(1, sizeof(struct animation));
    mustAllocate(a, "animation");

    a->whichSprite = whichSprite;
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

void drawEntity(struct entity* en, int* offset, ALLEGRO_BITMAP*** sprites){
    switch(en->behavior){
        case IDLE:
            if(en->dir)
                al_draw_bitmap(sprites[en->anim->whichSprite][IDLE_FRAME],
                floor(*offset + en->x), floor(en->y),
                0);
            else
                al_draw_scaled_bitmap(sprites[en->anim->whichSprite][IDLE_FRAME],
                0, 0,
                en->w, en->h,
                floor(*offset + en->x) + en->w, floor(en->y),
                en->w * -1, en->h,
                0);
            break;
        case WALKING:
            updateWalkFrame(en);
            if(en->dir)
                al_draw_bitmap(sprites[en->anim->whichSprite][en->anim->currentFrame],
                floor(*offset + en->x), floor(en->y),
                0);
            else
                al_draw_scaled_bitmap(sprites[en->anim->whichSprite][en->anim->currentFrame],
                0, 0,
                en->w, en->h,
                floor(*offset + en->x) + en->w, floor(en->y),
                en->w * -1, en->h,
                0);
            break;
        case JUMPING:
            if(en->dir)
                al_draw_bitmap(sprites[en->anim->whichSprite][JUMP_FRAME],
                floor(*offset + en->x), floor(en->y),
                0);
            else
                al_draw_scaled_bitmap(sprites[en->anim->whichSprite][JUMP_FRAME],
                0, 0,
                en->w, en->h,
                floor(*offset + en->x) + en->w, floor(en->y),
                en->w * -1, en->h,
                0);
            break;
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
                al_draw_bitmap(sprites[whichSprite],
                tiles[y][x].x + *offset,
                tiles[y][x].y,
                0);
            }
        }
    }
}