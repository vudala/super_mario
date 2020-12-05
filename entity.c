#include "level.h"
#include "entity.h"
#include "game.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct entity* newEntity(int x, int y, int dir, int behavior, struct animation* anim){
    struct entity* en = malloc(sizeof(struct entity));
    mustAllocate(en, "entity");

    en->x = x;
    en->y = y;
    en->dx = 0;
    en->dy = 0;
    en->dir = dir;
    en->w = al_get_bitmap_width(anim->frames[0]); // Usa as dimensoes do primeiro frame
    en->h = al_get_bitmap_height(anim->frames[0]); // Usa as dimensoes do primeiro frame
    en->behavior = behavior;
    en->anim = anim;

    return en;
}

void updateEntity(struct entity* en, struct tile** tiles){
    if(en->dir) en->dx = MONSTER_WALKING_SPEED;
    else en->dx = -MONSTER_WALKING_SPEED;
    
    switch(en->behavior){
        case IDLE: break;
        case JUMPING:
            en->dy += GRAVITY;
            if(tileDownCollision(en, tiles)) en->behavior = WALKING;
            tileUpCollision(en, tiles);
            break;
        case WALKING:
            if(!tileDownCollision(en, tiles)) en->behavior = JUMPING;
            break;
    }
    // Se bateu em algo troca de direção e continua andando
    if(tileLeftCollision(en, tiles)) {
        en->dir = RIGHT;
        en->behavior = WALKING;
    }
    if(tileRightCollision(en, tiles)){
        en->dir = LEFT;
        en->behavior = WALKING;
    }

    en->y += en->dy;
    en->x += en->dx;
}

void updateCharacter(struct entity* character, struct tile** tiles, struct entity** entities, unsigned char* key){
    switch(character->behavior){
        case IDLE:
            if (key[ALLEGRO_KEY_SPACE]){
                character->dy = JUMP_VELOCITY;
                character->behavior = JUMPING;
            } else
            if (key[ALLEGRO_KEY_LEFT]){
                character->dir = LEFT;
                character->dx = -PLAYER_WALKING_SPEED;
                character->behavior = WALKING;
            } else
            if (key[ALLEGRO_KEY_RIGHT]) {
                character->dir = RIGHT;
                character->dx = PLAYER_WALKING_SPEED;
                character->behavior = WALKING;
            } else
                character->dx = 0;

            
            tileUpCollision(character, tiles);
            tileLeftCollision(character, tiles);
            tileRightCollision(character, tiles);

            break;
        case JUMPING:
            if(key[ALLEGRO_KEY_LEFT]){
                character->dir = 0;
                character->dx = -PLAYER_WALKING_SPEED;
            } else
            if(key[ALLEGRO_KEY_RIGHT]){
                character->dir = 1;
                character->dx = PLAYER_WALKING_SPEED;
            } else character->dx = 0;
            
            character->dy += GRAVITY;
            
            
            if(character->dy >= 0){
                tileDownCollision(character, tiles);
            } else tileUpCollision(character, tiles);
            // Se estiver indo a direita
            if(character->dir) tileRightCollision(character, tiles);
            else tileLeftCollision(character, tiles);
            
            break;
        case WALKING:
            if (key[ALLEGRO_KEY_SPACE]) {
                character->dy = JUMP_VELOCITY;
                character->behavior = JUMPING;
            } else
            if (key[ALLEGRO_KEY_LEFT]) {
                character->dir = LEFT;
                character->dx = -PLAYER_WALKING_SPEED;
            } else
            if (key[ALLEGRO_KEY_RIGHT]) {
                character->dir = RIGHT;
                character->dx = PLAYER_WALKING_SPEED;
            } else {
                character->dx = 0;
                character->behavior = IDLE;
            }
            // Se não houver nada embaixo
            if(!tileDownCollision(character, tiles)) character->behavior = JUMPING;
            // Se estiver indo a direita
            if(character->dir) tileRightCollision(character, tiles);
            else tileLeftCollision(character, tiles);
            
            break;
    }

    character->y += character->dy;
    character->x += character->dx;
}