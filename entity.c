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
    // Flor, moeda, casco  ou checkpoint nasce parado
    if(enType == FLOWER || enType == COIN || enType == SHELL || enType == CHECKPOINT)
        behavior = IDLE;
    // Estrela nasce quicando
    else if(enType == STAR || enType == FIREBALL)
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
    if(en->lifeSpan == 0 || en->life == 0)
        return 1;
    if(!withinMapHeight(en) || !withinMapWidth(en))
        return 1;

    return 0;
}

void addFireball(struct entityList* fireballs, struct entity* en){
    // Onde irá nascer no eixo X
    int whichX = en->dir ? en->x + en->w : en->x - 1;
    int whichY =  floor(en->y + (en->w / 2));
    struct entity* newFireball = newEntity(FIREBALL, whichX, whichY,
                                    FIREBALL_SIZE, FIREBALL_SIZE, en->dir, 
                                    newAnimation(FIREBALL_SPRITE, WALK_START, WALK_END, WALK_DURATION),
                                    FIREBALL_SPAN
                                );
    insertEntity(fireballs, newFireball);
}

int fireballHit(struct entityList* fireballs, struct entity* en){
    struct entityNode* current = NULL;
    struct entityNode* next = fireballs->start; 

    while(next != NULL){
        current = next;
        next = current->next;
        // Se uma fireball bateu em um inimigo emove a fireball e retorna 1
        if(entityCollision(current->en, en) && (en->type == GOOMBA || en->type == TURTLE)){
            removeEntity(current->id, fireballs);
            return 1;
        }
    }

    return 0;
}

// Movimenta e atualiza as fireballs
void fireballsUpdate(struct entityList* fireballs, struct tile** tiles){
    struct entityNode* current = NULL;
    struct entityNode* next = fireballs->start; 

    while(next != NULL){
        current = next;
        next = current->next;
        updateEntity(current->en, tiles);
        if(isDead(current->en))
            removeEntity(current->id, fireballs);
    }
}

void updateEntity(struct entity* en, struct tile** tiles){
    en->lifeSpan -= 1;
    int whichSpeed = en->type == FIREBALL || en->type == SHELL ? FAST_SPEED : SLOW_SPEED;
    switch(en->behavior){
        case IDLE:
            if(!tileDownCollision(en, tiles)) 
                en->dy += GRAVITY;
            break;
        case BOUNCING:
            if(en->dir) en->dx = whichSpeed;
            else en->dx = -whichSpeed;

            en->dy += GRAVITY;

            if(tileDownCollision(en, tiles)) 
                en->dy = en->type == FIREBALL ? LOW_BOUNCE : HIGH_BOUNCE;

            tileUpCollision(en, tiles);
            // Se bateu em algo troca de direção
            if(tileLeftCollision(en, tiles) || tileRightCollision(en, tiles)){
                en->dir = !en->dir;
                if(en->type == FIREBALL)
                    en->life = 0; // Se for uma bola de fogo, a elimina
            }

            break;
        case JUMPING:
            if(en->dir) en->dx = whichSpeed;
            else en->dx = -whichSpeed;

            en->dy += GRAVITY;
            
            if(tileDownCollision(en, tiles)) en->behavior = WALKING;
            tileUpCollision(en, tiles);
            // Se bateu em algo troca de direção
            if(tileLeftCollision(en, tiles)) en->dir = RIGHT;
            if(tileRightCollision(en, tiles)) en->dir = LEFT;

            break;
        case WALKING:
            if(en->dir) en->dx = whichSpeed;
            else en->dx = -whichSpeed;

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