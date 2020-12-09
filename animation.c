#include "animation.h"
#include "entity.h"
#include "utils.h"
#include "level.h"
#include "game.h"

#include <math.h>

ALLEGRO_BITMAP** loadEntitySprite(char* path){
    ALLEGRO_BITMAP* mainBitmap = al_load_bitmap(path);
   
    int width = al_get_bitmap_width(mainBitmap);
    int height = al_get_bitmap_height(mainBitmap);
    int frameWidth = width / FRAMES_N;

    ALLEGRO_BITMAP** sprites = calloc(FRAMES_N, sizeof(ALLEGRO_BITMAP*));
    mustAllocate(sprites, "sprites");

    ALLEGRO_BITMAP* subBitmap = NULL;

    for(int i = 0; i < FRAMES_N; i++){
        subBitmap = al_create_sub_bitmap(mainBitmap, i * frameWidth, 0, frameWidth, height);
        mustAllocate(subBitmap, path);
        sprites[i] = al_clone_bitmap(subBitmap);
    }

    al_destroy_bitmap(mainBitmap);
    
    return sprites;
}

ALLEGRO_BITMAP** loadTileSprites(){
    ALLEGRO_BITMAP** sprites = calloc(TILE_SPRITES_N, sizeof(ALLEGRO_BITMAP*));
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

ALLEGRO_BITMAP*** loadSprites(){
    ALLEGRO_BITMAP*** sprites = calloc(ENTITY_SPRITES_N, sizeof(ALLEGRO_BITMAP**));
    mustAllocate(sprites, "sprites");

    sprites[CHAR_SPRITE] = loadEntitySprite("resources/sprites/entities/mario.png");
    sprites[SMALL_CHAR_SPRITE] = loadEntitySprite("resources/sprites/entities/mario_small.png");
    sprites[CHAR_FLOWER_SPRITE] = loadEntitySprite("resources/sprites/entities/mario_flower.png");
    sprites[GOOMBA_SPRITE] = loadEntitySprite("resources/sprites/entities/goomba.png");
    sprites[TURTLE_SPRITE] = loadEntitySprite("resources/sprites/entities/turtle.png");
    sprites[FLOWER_SPRITE] = loadEntitySprite("resources/sprites/entities/flower.png");
    sprites[STAR_SPRITE] = loadEntitySprite("resources/sprites/entities/star.png");
    sprites[SHELL_SPRITE] = loadEntitySprite("resources/sprites/entities/shell.png");
    sprites[MUSHROOM_SPRITE] = loadEntitySprite("resources/sprites/entities/mushroom.png");
    sprites[COIN_SPRITE] = loadEntitySprite("resources/sprites/entities/coin.png");
    sprites[FIREBALL_SPRITE] = loadEntitySprite("resources/sprites/entities/fireball.png");

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