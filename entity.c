#include "level.h"
#include "entity.h"
#include "game.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct entity* newEntity(int x, int y, int w, int h, int dir, int behavior, struct animation* anim){
    struct entity* en = malloc(sizeof(struct entity));
    mustAllocate(en, "entity");

    en->x = x;
    en->y = y;
    en->w = w;
    en->h = h;
    en->dx = 0;
    en->dy = 0;
    en->dir = dir;
    en->behavior = behavior;
    en->anim = anim;
    en->life = 1;

    return en;
}

int entityCollision(struct entity* en1, struct entity* en2){
    if(en1->x > en2->x + en2->w) return 0;
    if(en1->x + en1->w < en2->x ) return 0;
    if(en1->y > en2->y + en2->h) return 0;
    if(en1->y + en1->h < en2->y) return 0;

    return 1;
}

int entityLeftCollision(struct entity* en1, struct entity* en2){
    if(entityCollision(en1, en2)) // Se colidiu
        // Se en1 estiver a direita de en2
        if(en1->x + en1->w > en2->x + en2->w) 
            return 1;
    return 0;
}

int entityRightCollision(struct entity* en1, struct entity* en2){
    if(entityCollision(en1, en2)) // Se colidiu
        // Se en1 estiver a esquerda de en2
        if(en1->x + en1->w < en2->x + en2->w) 
            return 1;
    return 0;
}

int entityDownCollision(struct entity* en1, struct entity* en2){
    if(entityCollision(en1, en2)) // Se colidiu
        // Se en1 estiver a acima de en2
        if(en1->y + en1->h < en2->y + en2->h) 
            return 1;
    return 0;
}

int entityUpCollision(struct entity* en1, struct entity* en2){
    if(entityCollision(en1, en2)) // Se colidiu
        // Se en1 estiver a abaixo de en2
        if(en1->y > en2->y) 
            return 1;
    return 0;
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

void updateCharacter(struct entity* character, struct tile** tiles, unsigned char* key){
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