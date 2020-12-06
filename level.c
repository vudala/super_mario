#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "level.h"
#include "game.h"
#include "entity.h"
#include "entity_list.h"
#include "animation.h"
#include <math.h>

struct tile* newTile(int x, int y, int w, int h, int active, int type){
    struct tile* t = calloc(1, sizeof(struct tile));
    mustAllocate(t, "tile");

    t->x = x;
    t->y = y;
    t->w = w;
    t->h = h;
    t->active = active;
    t->type = type;

    return t;
}

int getTileV(char c){
    if(c == EMPTY_BLOCK) return EMPTY_BLOCK_V; 
    if(c == BRICK_BLOCK) return BRICK_BLOCK_V;
    if(c == HARD_BRICK_BLOCK) return HARD_BRICK_BLOCK_V;
    if(c == PIPE_BLOCK) return PIPE_BLOCK_V;
    if(c == PIPE_TOP_BLOCK) return PIPE_TOP_BLOCK_V;
    if(c == COIN_BLOCK) return COIN_BLOCK_V;
    if(c == STAR_BLOCK) return STAR_BLOCK_V;
    if(c == MUSHROOM_BLOCK) return MUSHROOM_BLOCK_V;
    if(c == FLOWER_BLOCK) return FLOWER_BLOCK_V;
    return EMPTY_BLOCK_V;
}


struct tile* pointToTile(int x, int y, struct tile** tiles){
    if (x < 0 || y < 0 || x > MAP_WIDTH * TILE_WIDTH || y > MAP_HEIGHT * TILE_HEIGHT) return NULL;
    int i = floor(y / TILE_HEIGHT); // Linha da tile
    int j = floor(x / TILE_WIDTH); // Coluna da tile
    return &tiles[i][j];
}



int tileLeftCollision(struct entity* en, struct tile** tiles){
    if(en->dx < 0){
        struct tile* left1 = pointToTile(en->x-1, en->y, tiles);
        struct tile* left2 = pointToTile(en->x-1, en->y+en->h - 1, tiles);
        if(!left1 || !left2) return 0; // Se for outbounds
        if(left1->active || left2->active){
            en->dx = 0;
            en->x = left1->x+left1->w;
            if(en->behavior != JUMPING) en->behavior = IDLE;
            return 1;
        }
    }
    return 0;
}

int tileRightCollision(struct entity* en, struct tile** tiles){
    if(en->dx > 0){
        struct tile* right1 = pointToTile(en->x+en->w, en->y, tiles);
        struct tile* right2 = pointToTile(en->x+en->w, en->y+en->h - 1, tiles);
        if(!right1 || !right2) return 0; // Se for outbounds
        if(right1->active || right2->active){
            en->dx = 0;
            en->x = right1->x - right1->w;
            if(en->behavior != JUMPING) en->behavior = IDLE;
            return 1;
        }
    }
    return 0;
}

int tileUpCollision(struct entity* en, struct tile** tiles/*, struct entityList* entities*/){
    struct tile* up1 = pointToTile(en->x + 5, en->y, tiles);
    struct tile* up2 = pointToTile(en->x+en->w - 5, en->y, tiles);
    if(!up1 || !up2) return 0; // Se for outbounds
    if(up1->active || up2->active){
        en->dy = 0;
        en->y = up1->y + up1->h;
        // if(up1->active){
        //     switch(up1->type){
        //         case COIN_BLOCK:case STAR_BLOCK:case MUSHROOM_BLOCK:case FLOWER_BLOCK:
        //             insertEntity(entities, 
        //             newEntity()
        //             );
        //             break;
        //     }
        // }
        return 1;
    }
    
    return 0;
}

int tileDownCollision(struct entity* en, struct tile** tiles){
    struct tile* down1 = pointToTile(en->x + 5, en->y+en->h, tiles);
    struct tile* down2 = pointToTile(en->x+en->w - 5, en->y+en->h, tiles);
    if(!down1 || !down2) return 0; // Se for outbounds
    if(down1->active || down2->active){
        if(en->behavior == JUMPING){
            en->behavior = IDLE;
            en->dy = 0;
            en->y = down1->y - en->h;
        }
        return 1;
    }
    
    return 0;
}

struct tile** load_level(char* levelPath, struct entityList* l, ALLEGRO_BITMAP*** sprites){
    FILE* file = fopen(levelPath, "r");
    mustAllocate(file, levelPath);
    struct tile** t = (struct tile**) allocateMatrix(sizeof(struct tile), MAP_WIDTH, MAP_HEIGHT);
    mustAllocate(t, "tiles");

    struct tile* newT = NULL;
    int whichSprite = 0;
    int width, height;

    char c;
    int i = 0, j = 0;
    while((c = fgetc(file)) != EOF){
        if(j == MAP_WIDTH){
            fgetc(file);
            i++;
            j = 0;
        }
        switch(c){
            case '\n': break;
            case MAIN_CHARACTER: case GOOMBA: case TURTLE:
                whichSprite = typeToSpriteID(c);
                width = al_get_bitmap_width(sprites[whichSprite][0]);
                height = al_get_bitmap_height(sprites[whichSprite][0]);
                insertEntity(l,
                    newEntity(j*TILE_WIDTH, i*TILE_HEIGHT, width, height, RIGHT, JUMPING, 
                        newAnimation(whichSprite, 2, FRAME_DURATION))
                );
                newT = newTile( j * TILE_WIDTH, i * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, 0, EMPTY_BLOCK);
                t[i][j] = *newT;
                free(newT);
                break;
            default:
                newT = newTile( j * TILE_WIDTH, i * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, c != EMPTY_BLOCK ? 1 : 0, c);
                t[i][j] = *newT;
                free(newT);
                break;
        }
        j++;
    }

    fclose(file);

    return t;
}