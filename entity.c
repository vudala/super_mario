#include "level.h"
#include "entity.h"
#include "game.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct tile* pointToTile(int x, int y, struct tile** tiles){
    if (x < 0 || y < 0 || x > MAP_WIDTH * TILE_WIDTH || y > MAP_HEIGHT * TILE_HEIGHT) return NULL;
    int i = floor(y / TILE_HEIGHT); // Linha da tile
    int j = floor(x / TILE_WIDTH); // Coluna da tile
    return &tiles[i][j];
}

struct entity* newEntity(int x, int y, int dx, int dy, int dir, int w, int h, int behavior){
    struct entity* en = malloc(sizeof(struct entity));
    mustAllocate(en, "entity");

    en->x = x;
    en->y = y;
    en->dx = dx;
    en->dy = dy;
    en->dir = dir;
    en->w = w;
    en->h = h;
    en->behavior = behavior;

    return en;
}


int checkLeftCollision(struct entity* en, struct tile** tiles){
    if(en->dx < 0){
        struct tile* left1 = pointToTile(en->x-1, en->y, tiles);
        struct tile* left2 = pointToTile(en->x-1, en->y+en->h - 1, tiles);
        if(!left1 || !left2) return 0; // Se for outbounds
        if(left1->active || left2->active){
            en->dx = 0;
            if(en->behavior != JUMPING){
                en->x = left1->x+left1->w;
                en->behavior = IDLE;
            }
            return 1;
        } 
    }
    return 0;
}

int checkRightCollision(struct entity* en, struct tile** tiles){
    if(en->dx > 0){
        struct tile* right1 = pointToTile(en->x+en->w+1, en->y, tiles);
        struct tile* right2 = pointToTile(en->x+en->w+1, en->y+en->h - 1, tiles);
        if(!right1 || !right2) return 0; // Se for outbounds
        if(right1->active || right2->active){
            en->dx = 0;
            if(en->behavior != JUMPING){
                en->x = right1->x - right1->w;
                en->behavior = IDLE;
            } 
            return 1;
        }
    }
    return 0;
}

int checkDownCollision(struct entity* en, struct tile** tiles){
    if(en->dy > 0){
        struct tile* down1 = pointToTile(en->x, en->y+en->h, tiles);
        struct tile* down2 = pointToTile(en->x+en->w - 1, en->y+en->h, tiles);
        if(!down1 || !down2) return 0; // Se for outbounds
        if(down1->active || down2->active){
            if(en->behavior == JUMPING){
                en->behavior = IDLE;
                en->dy = 0;
                en->y = down1->y - en->h;
            }
            return 1;
        }
    }
    return 0;
}

int checkUpCollision(struct entity* en, struct tile** tiles){
    if(en->dy < 0 ){
        struct tile* up1 = pointToTile(en->x, en->y, tiles);
        struct tile* up2 = pointToTile(en->x+en->w-1, en->y, tiles);
        if(!up1 || !up2) return 0; // Se for outbounds
        if(up1->active || up2->active){
            en->dy = 0;
            en->y = up1->y + up1->h;
            return 1;
        }
    }
    return 0;
}