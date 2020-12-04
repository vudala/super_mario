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

ALLEGRO_BITMAP** newAnimation(char* idlePath, char* walk1Path, char* walk2Path, char* jumpPath){
    ALLEGRO_BITMAP** a = calloc(4, sizeof(ALLEGRO_BITMAP*));
    mustAllocate(a, "animation");

    a[0] = al_load_bitmap(idlePath);
    mustAllocate(&a[0], "animation");
    a[1] = al_load_bitmap(walk1Path);
    mustAllocate(&a[1], "animation");
    a[2] = al_load_bitmap(walk2Path);
    mustAllocate(&a[2], "animation");
    a[3] = al_load_bitmap(jumpPath);
    mustAllocate(&a[3], "animation");

    return a;
}


struct entity* newEntity(int x, int y, int dx, int dy, int dir, int w, int h, int behavior, ALLEGRO_BITMAP** animation){
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
    en->animation = animation;

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
    }
    return 0;
}

int checkUpCollision(struct entity* en, struct tile** tiles){
    if(en->dy < 0 ){
        struct tile* up1 = pointToTile(en->x + 5, en->y, tiles);
        struct tile* up2 = pointToTile(en->x+en->w - 5, en->y, tiles);
        if(!up1 || !up2) return 0; // Se for outbounds
        if(up1->active || up2->active){
            en->dy = 0;
            en->y = up1->y + up1->h;
            return 1;
        }
    }
    return 0;
}

void drawEntity(struct entity* en, int offset){
    switch(en->behavior){
        case IDLE:
            if(en->dir)
                al_draw_bitmap(en->animation[0],
                floor(offset + en->x), floor(en->y),
                0);
            else
                al_draw_scaled_bitmap(en->animation[0],
                0, 0,
                en->w, en->h,
                floor(offset + en->x) + en->w, floor(en->y),
                en->w * -1, en->h,
                0);
            break;
        case WALKING:
            if(en->dir)
                al_draw_bitmap(en->animation[1],
                floor(offset + en->x), floor(en->y),
                0);
            else
                al_draw_scaled_bitmap(en->animation[1],
                0, 0,
                en->w, en->h,
                floor(offset + en->x) + en->w, floor(en->y),
                en->w * -1, en->h,
                0);
            break;
        case JUMPING:
            if(en->dir)
                al_draw_bitmap(en->animation[3],
                floor(offset + en->x), floor(en->y),
                0);
            else
                al_draw_scaled_bitmap(en->animation[3],
                0, 0,
                en->w, en->h,
                floor(offset + en->x) + en->w, floor(en->y),
                en->w * -1, en->h,
                0);
            break;
    }
}

void updateCharacter(struct entity* character, struct tile** tiles, unsigned char* key){

    switch(character->behavior){
        case IDLE:
            if (key[ALLEGRO_KEY_SPACE]){
                character->dy = JUMP_VELOCITY;
                character->behavior = JUMPING;
            } else
            if (key[ALLEGRO_KEY_LEFT]){
                character->dir = LEFT;
                character->dx = -WALKING_SPEED;
                character->behavior = WALKING;
            } else
            if (key[ALLEGRO_KEY_RIGHT]) {
                character->dir = RIGHT;
                character->dx = WALKING_SPEED;
                character->behavior = WALKING;
            } else
                character->dx = 0;

            checkLeftCollision(character, tiles);
            checkRightCollision(character, tiles);
            checkUpCollision(character, tiles);

            break;
        case JUMPING:
            if(key[ALLEGRO_KEY_LEFT]){
                character->dir = 0;
                character->dx = -WALKING_SPEED;
            } else
            if(key[ALLEGRO_KEY_RIGHT]){
                character->dir = 1;
                character->dx = WALKING_SPEED;
            } else character->dx = 0;
            
            character->dy += GRAVITY;
            
            checkLeftCollision(character, tiles);
            checkRightCollision(character, tiles);
            checkDownCollision(character, tiles);
            checkUpCollision(character, tiles);
            
            break;
        case WALKING:
            if (key[ALLEGRO_KEY_SPACE]) {
                character->dy = JUMP_VELOCITY;
                character->behavior = JUMPING;
            } else
            if (key[ALLEGRO_KEY_LEFT]) {
                character->dir = LEFT;
                character->dx = -WALKING_SPEED;
            } else
            if (key[ALLEGRO_KEY_RIGHT]) {
                character->dir = RIGHT;
                character->dx = WALKING_SPEED;
            } else {
                character->dx = 0;
                character->behavior = IDLE;
            }

            checkLeftCollision(character, tiles);
            checkRightCollision(character, tiles);
            if(!checkDownCollision(character, tiles))  character->behavior = JUMPING;
            
            break;
    }

    character->y += character->dy;
    character->x += character->dx;
}