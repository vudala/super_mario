#include "character.h"
#include "utils.h"
#include "sound.h"
#include "entity.h"

struct character* newCharacter(struct entity* en){
    struct character* newC = calloc(1, sizeof(struct character));
    mustAllocate(newC, "new character");

    // Os outros atributos já vem como 0, por causa do calloc
    newC->self = en;
    newC->skillCooldown = FIREBALL_COOLDOWN;

    return newC;
};

struct tile* updateCharacter(struct character* character, struct tile** tiles, ALLEGRO_SAMPLE** samples,
struct entityList* fireballs){
    struct tile* rValue = NULL; // Valor de retorno
    character->invincibility -= 1;
    if(character->invincibility == 0) character->star = 0;

    // Se estiver sob poder da flor e o tempo de recarga esgotado
    // Ao apertar espaço solta uma bola de fogo
    if (key[ALLEGRO_KEY_SPACE] && character->currClock < 1 && character->power == FLOWER_POWER){
        character->currClock = character->skillCooldown;
        addFireball(fireballs, character->self);
        al_play_sample(samples[FIREBALL_SAMPLE], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    character->currClock -= 1;

    switch(character->self->behavior){
        case IDLE:  
            if (key[ALLEGRO_KEY_UP]){ // Pula
                character->self->dy = JUMP_VELOCITY;
                character->self->behavior = JUMPING;
                al_play_sample(samples[JUMP_SAMPLE], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
            else if (key[ALLEGRO_KEY_LEFT]){ // Anda pra esquerda
                character->self->dir = LEFT;
                character->self->dx = -CHARACTER_SPEED;
                character->self->behavior = WALKING;
            } 
            else if (key[ALLEGRO_KEY_RIGHT]) { // Anda pra direita
                character->self->dir = RIGHT;
                character->self->dx = CHARACTER_SPEED;
                character->self->behavior = WALKING;
            } 
            else
                character->self->dx = 0; // Para de acelerar
            
            tileUpCollision(character->self, tiles);
            // Se colidiu lateralmente
            if(tileLeftCollision(character->self, tiles) || tileRightCollision(character->self, tiles))
                character->self->behavior = IDLE;

            break;
        case JUMPING:
            if(key[ALLEGRO_KEY_LEFT]){ // Se move para a esquerda
                character->self->dir = LEFT;
                character->self->dx = -CHARACTER_SPEED;
            } 
            else if(key[ALLEGRO_KEY_RIGHT]){ // Se move para a direita
                character->self->dir = RIGHT;
                character->self->dx = CHARACTER_SPEED;
            } 
            else
                character->self->dx = 0; // Para de acelerar
            
            character->self->dy += GRAVITY; // É puxado pela gravidade
            
            if(character->self->dy >= 0){
                if(tileDownCollision(character->self, tiles))
                    character->self->behavior = IDLE;
            }
            else {
                // Se colidiu acima, checa se é um bloco especial o retorna
                struct tile* aux = tileUpCollision(character->self, tiles);
                if (aux) switch(aux->type){
                    case COIN_BLOCK:case STAR_BLOCK:case MUSHROOM_BLOCK:case FLOWER_BLOCK:
                    case SECRET_BLOCK:
                            rValue = aux;
                }        
            }

            tileRightCollision(character->self, tiles);
            tileLeftCollision(character->self, tiles);
            
            break;
        case WALKING:
            if (key[ALLEGRO_KEY_UP]) { // Pula 
                character->self->dy = JUMP_VELOCITY;
                character->self->behavior = JUMPING;
            } 
            else if (key[ALLEGRO_KEY_LEFT]) { // Anda a esquerda
                character->self->dir = LEFT;
                character->self->dx = -CHARACTER_SPEED;
            } 
            else if (key[ALLEGRO_KEY_RIGHT]) { // Anda a direita
                character->self->dir = RIGHT;
                character->self->dx = CHARACTER_SPEED;
            } else { // Para
                character->self->dx = 0;
                character->self->behavior = IDLE;
            }

            // Se não houver nada embaixo
            if(!tileDownCollision(character->self, tiles))
                character->self->behavior = JUMPING;

            // Se colidiu lateralmente
            if(tileRightCollision(character->self, tiles) || tileLeftCollision(character->self, tiles))
                character->self->behavior = IDLE;
            
            break;
    }
    
    character->self->y += character->self->dy;
    character->self->x += character->self->dx;

    return rValue;
}

struct entity* tilesInteract(struct character* character, struct tile** tiles,
ALLEGRO_SAMPLE** samples, struct entityList* fireballs, int* score){
    struct entity* newEn = NULL;
    struct tile* t = updateCharacter(character, tiles, samples, fireballs);

    if(t){
        if(t->content < 1) return NULL; // Se os contéudos de t ja se esgotaram
        switch (t->type){
            case COIN_BLOCK: *score += COIN_SCORE;                
            case STAR_BLOCK:case MUSHROOM_BLOCK:case FLOWER_BLOCK:;
                int enType = specialTileContent(t->type);
                int whichSprite = entitySpriteID(enType);

                newEn = newEntity(
                    enType, t->x, t->y - t->h, t->w, t->h,
                    !character->self->dir, // Aponta para a direção oposta do personagem
                    newAnimation(whichSprite, WALK_START, WALK_END, WALK_DURATION),
                    enType == COIN ? 10 : INFINITE // Se for moeda, ela tem uma duração limitada
                );
                
                if(t->type == COIN_BLOCK)
                    al_play_sample(samples[COIN_SAMPLE], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                else
                    al_play_sample(samples[POWERUP_SAMPLE], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

                t->content -= 1;
                break;
            case SECRET_BLOCK:
                al_play_sample(samples[SECRET_SAMPLE], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                break;
        }
    }

    return newEn;
}

void givePower(struct character* character, int powerType, ALLEGRO_SAMPLE** samples, int* score){
    switch(powerType){
        case MUSHROOM_POWER:
            if(character->power == NO_POWER){
                character->self->w = BIG_WIDTH;
                character->self->h = BIG_HEIGHT;
                character->power = MUSHROOM_POWER;
                character->self->anim->sprite = CHAR_SPRITE;
                character->self->behavior = JUMPING;
                al_play_sample(samples[POWERUP_SAMPLE], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
            else
                *score += BUFF_SCORE;
            break; 
        case FLOWER_POWER:
            if(character->power != FLOWER_POWER){
                character->self->w = BIG_WIDTH;
                character->self->h = BIG_HEIGHT;
                character->power = FLOWER_POWER;
                character->self->anim->sprite = CHAR_FLOWER_SPRITE;
                character->self->behavior = JUMPING;
                al_play_sample(samples[POWERUP_SAMPLE], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
            else
                *score += BUFF_SCORE;
            break;
        case STAR_POWER:
            character->invincibility = INVINCIBILTY;
            character->star = 1;
            al_play_sample(samples[POWERUP_SAMPLE], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            break;
        
        case NO_POWER:
            character->power = NO_POWER;
            character->self->w = SMALL_WIDTH;
            character->self->h = SMALL_HEIGHT;
            character->self->anim->sprite = SMALL_CHAR_SPRITE;
            character->self->behavior = JUMPING;
            al_play_sample(samples[UNPOWER_SAMPLE], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            break;
    }
}

// Realiza a interação entre o personagem e uma entidade qualquer
// Se acabou o jogo retorna 1, do contrário 0
int entityInteract(struct character* character, struct entityList* entities, struct entityNode* current,
ALLEGRO_SAMPLE** samples, int* score){
    struct entity* newEn = NULL;
    switch(current->en->type){
        case CHECKPOINT: // Se for o checkpoint, termina o jogo
            al_play_sample(samples[CHECKPOINT_SAMPLE], 15.0, 1.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
            return 1;
        case MUSHROOM: case FLOWER: case STAR:// Se for um power up, da poder ao personagem
            givePower(character, current->en->type, samples, score);
            removeEntity(current->id, entities);
            break;
        default:
            if(entityDownCollision(character->self, current->en)){ // Se pulo em cima
                switch(current->en->type){
                    case SHELL: // Se for casco o para
                        current->en->behavior = IDLE;
                        current->en->dx = 0;
                        character->self->dy = LOW_BOUNCE;
                        break;
                    case TURTLE: // Se for tartaruga cria um casco
                        newEn = newEntity(
                            SHELL, current->en->x, current->en->y,
                            current->en->w, current->en->h / 2,
                            current->en->dir,
                            newAnimation(SHELL_SPRITE, 0, FRAMES_N-1, WALK_DURATION),
                            INFINITE
                        );
                        newEn->dy += GRAVITY;
                        insertEntity(entities, newEn);
                    default: // Mata a entidade
                        *score += KILL_SCORE;
                        removeEntity(current->id, entities);
                        character->self->dy = HIGH_BOUNCE;
                        character->self->behavior = JUMPING;
                        al_play_sample(samples[BOUNCE_SAMPLE], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        break;
                }
            } // Se esbarrar num casco parado enquanto anda, o chuta
            else if(current->en->behavior == IDLE && character->self->behavior == WALKING &&
                    current->en->type == SHELL){
                current->en->behavior = WALKING;
                current->en->dir = character->self->dir;
                current->en->x += current->en->dir ? (current->en->w / 2) : -(current->en->w / 2);
            } // Se estiver invencivel nao morre
            else if (character->invincibility > 0){
                if(character->star){ // Se estiver sob poder da estrela, mata a entidade
                    *score += KILL_SCORE;
                    removeEntity(current->id, entities);
                    al_play_sample(samples[HIT_SAMPLE], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            } // Se estiver com algum poder, perde o poder e fica brevemente invencivel
            else if (character->power != NO_POWER){
                character->invincibility = HIT_SPAN;
                givePower(character, NO_POWER, samples, NULL);
            } // Do contrário tomou um hit, acaba o jogo
            else{
                al_play_sample(samples[DEATH_SAMPLE], 1.0, 1.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
                return 1;
            }
                
                
            break;
    }

    return 0;
}    

int entitiesInteract(struct character* character, struct tile** tiles, struct entityList* entities,
struct entityList* fireballs, ALLEGRO_SAMPLE** samples, int* score){
    if(character->self->y + character->self->h > VIRTUAL_HEIGHT)
        return 1;

    fireballsUpdate(fireballs, tiles);

    struct entityNode* current = NULL;
    struct entityNode* next = entities->start; 

    // Navega por todas a entidades e reaje conforme o tipo de colisão e o tipo das entidades
    while(next != NULL){
        current = next;
        next = current->next;
        // Se ela morreu
        if(isDead(current->en)) // Se ela estiver com os atributos de um morto
            removeEntity(current->id, entities);
        else if(fireballHit(fireballs, current->en)){ // Se uma bola de fogo a atingiu
            removeEntity(current->id, entities);
            al_play_sample(samples[HIT_SAMPLE], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        }
        else {
            updateEntity(current->en, tiles);
            if(entityCollision(character->self, current->en))
                return entityInteract(character, entities, current, samples, score);
        }        
    }

    return 0;
}