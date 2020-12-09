#include "entity.h"
#include "utils.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void destroyEntity(struct entity* en){
    free(en->anim);
    free(en);
}

int whichBehavior(int enType){
    // Se for qualquer uma das outras, nasce pulando
    int behavior = JUMPING;
    // Se flor moeda ou casco, ela nasce parada
    if(enType == FLOWER || enType == COIN || enType == SHELL)
        behavior = IDLE;
    // Se for estrela, nasce quicando
    else if(enType == STAR)
        behavior = BOUNCING;

    return behavior;
}

struct entity* newEntity(int type, int x, int y, int w, int h, int dir, struct animation* anim, int lSpan){
    struct entity* en = malloc(sizeof(struct entity));
    mustAllocate(en, "entity");

    en->type = type;
    en->x = x;
    en->y = y;
    en->w = w;
    en->h = h;
    en->dx = 0;
    en->dy = 0;
    en->dir = dir;
    en->behavior = whichBehavior(type);
    en->anim = anim;
    en->life = 1;
    en->lifeSpan = lSpan;
    // en->power = NO_POWER; // Começa sem poderes
    // en->invincibility = 0; // Começa sem invencibilidade
   
    return en;
}

int entityCollision(struct entity* en1, struct entity* en2){
    if(en1->x > en2->x + en2->w) return 0;
    if(en1->x + en1->w < en2->x ) return 0;
    if(en1->y > en2->y + en2->h) return 0;
    if(en1->y + en1->h < en2->y) return 0;

    return 1;
}

int entityDownCollision(struct entity* en1, struct entity* en2){
    if(entityCollision(en1, en2)) // Se colidiu
        // Se en1 estiver a acima de en2
        if(en1->y + en1->h < en2->y + en2->h) 
            return 1;
    return 0;
}


int isDead(struct entity* en){
    if(en->y > MAP_HEIGHT * TILE_HEIGHT)
        return 1;
    if(en->x < 0)
        return 1;
    if(en->lifeSpan == 0 || en->life == 0) return 1;
    return 0;
}

void updateEntity(struct entity* en, struct tile** tiles){
    en->lifeSpan -= 1;

    switch(en->behavior){
        case BOUNCING:
            if(en->dir) en->dx = MONSTER_WALKING_SPEED;
            else en->dx = -MONSTER_WALKING_SPEED;

            en->dy += GRAVITY;

            if(tileDownCollision(en, tiles)) en->dy = BOUNCE_VELOCITY;
            tileUpCollision(en, tiles);

            // Se bateu em algo troca de direção
            if(tileLeftCollision(en, tiles)) en->dir = RIGHT;
            if(tileRightCollision(en, tiles)) en->dir = LEFT;
            break;
        case JUMPING:
            if(en->dir) en->dx = MONSTER_WALKING_SPEED;
            else en->dx = -MONSTER_WALKING_SPEED;

            en->dy += GRAVITY;
            
            if(tileDownCollision(en, tiles)) en->behavior = WALKING;
            tileUpCollision(en, tiles);
            // Se bateu em algo troca de direção
            if(tileLeftCollision(en, tiles)) en->dir = RIGHT;
            if(tileRightCollision(en, tiles)) en->dir = LEFT;

            break;
        case WALKING:
            if(en->dir) en->dx = MONSTER_WALKING_SPEED;
            else en->dx = -MONSTER_WALKING_SPEED;
            // Se não houver tiles embaixo
            if(!tileDownCollision(en, tiles)) en->behavior = JUMPING;
            // Se bateu em algo troca de direção
            if(tileLeftCollision(en, tiles)) en->dir = RIGHT;
            if(tileRightCollision(en, tiles)) en->dir = LEFT;
            break;
    }
    
    en->y += en->dy;
    en->x += en->dx;
}