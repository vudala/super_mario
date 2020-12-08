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
   
    return en;
}

int entityCollision(struct entity* en1, struct entity* en2){
    if(en1->x > en2->x + en2->w) return 0;
    if(en1->x + en1->w < en2->x ) return 0;
    if(en1->y > en2->y + en2->h) return 0;
    if(en1->y + en1->h < en2->y) return 0;

    return 1;
}

// int entityLeftCollision(struct entity* en1, struct entity* en2){
//     if(entityCollision(en1, en2)) // Se colidiu
//         // Se en1 estiver a direita de en2
//         if(en1->x + en1->w > en2->x + en2->w) 
//             return 1;
//     return 0;
// }

// int entityRightCollision(struct entity* en1, struct entity* en2){
//     if(entityCollision(en1, en2)) // Se colidiu
//         // Se en1 estiver a esquerda de en2
//         if(en1->x + en1->w < en2->x + en2->w) 
//             return 1;
//     return 0;
// }

// int entityUpCollision(struct entity* en1, struct entity* en2){
//     if(entityCollision(en1, en2)) // Se colidiu
//         // Se en1 estiver a abaixo de en2
//         if(en1->y > en2->y) 
//             return 1;
//     return 0;
// }

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

struct tile* updateCharacter(struct entity* character, struct tile** tiles, unsigned char* key){
    struct tile* rValue = NULL; // Valor de retorno

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
            if(tileLeftCollision(character, tiles) || tileRightCollision(character, tiles))
                character->behavior = IDLE;

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
            // Se estiver caindo
            if(character->dy >= 0){
                if(tileDownCollision(character, tiles))
                    character->behavior = IDLE;
            }
            else {
                // Se colidiu acima, checa se é um bloco especial o retorna
                struct tile* aux = tileUpCollision(character, tiles);
                if (aux) switch(aux->type){
                    case COIN_BLOCK:case STAR_BLOCK:case MUSHROOM_BLOCK:case FLOWER_BLOCK:
                            rValue = aux;
                }        
            }

            tileRightCollision(character, tiles);
            tileLeftCollision(character, tiles);
            
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
            if(!tileDownCollision(character, tiles))
                character->behavior = JUMPING;

            // Se estiver indo a direita
            if(tileRightCollision(character, tiles) || tileLeftCollision(character, tiles))
                character->behavior = IDLE;
            
            break;
    }

    character->y += character->dy;
    character->x += character->dx;

    return rValue;
}

/* Realiza a interação do personagem com as tiles,
colide e retorna uma nova entidade caso seja um bloco especial */
struct entity* tilesInteract(struct entity* character, struct tile** tiles, unsigned char* key){
    struct entity* newEn = NULL;
    struct tile* t = updateCharacter(character, tiles, key);

    if(t){
        switch (t->type){
            case COIN_BLOCK:case STAR_BLOCK:case MUSHROOM_BLOCK:case FLOWER_BLOCK:;
                int enType = specialTileContent(t->type);
                int whichSprite = entitySpriteID(enType);
                newEn = newEntity(enType, t->x, t->y - t->h, t->w, t->h,
                    !character->dir, // Aponta para a direção oposta do personagem
                    newAnimation(whichSprite, 2, FRAME_DURATION),
                    enType == COIN ? 10 : -1 // Se for moeda, ela tem uma duração limitada
                );
                break;
        }   
    }

    return newEn;
}

int entitiesInteract(struct entity* character, struct tile** tiles, struct entityList* entities){
    struct entityNode* next = entities->start;

    while(next != NULL){
        updateEntity(next->en, tiles);
        if(isDead(next->en)) removeEntity(next->id, entities);
        // Se pulou em cima de um inimigo
        if(entityDownCollision(character, next->en)){
            next->en->life -= 1;
            character->dy = BOUNCE_VELOCITY;
            character->behavior = JUMPING;
            break;
        } else if(entityCollision(character, next->en)){
            if(next->en->type == MUSHROOM){
                //giveMushroomPower(character);
                removeEntity(next->id, entities);
            }
            else if (next->en->type == FLOWER){
                //giveFlowerPower(character);
                removeEntity(next->id, entities);
            }   
            else if (next->en->type == STAR){
                //giveStarPower(character);
                removeEntity(next->id, entities);
            } else // Se não for um buff, te mata
                return 1;
        }
        next = next->next;
    }

    return 0;
}

int gameUpdate(struct entity* character, struct tile** tiles, struct entityList* entities, unsigned char* key){
    struct entity* newEn = tilesInteract(character, tiles, key);
    if(newEn) insertEntity(entities, newEn);
    newEn = NULL;

    return entitiesInteract(character, tiles, entities);
}