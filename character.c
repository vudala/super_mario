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

void move(struct entity* en, int xspeed, int direction){
    en->dir = direction;
    en->dx = direction ? xspeed : -xspeed;
}

void walk(struct entity* en, int xspeed, int direction){
    move(en, xspeed, direction);
    en->behavior = WALKING;
}

void jump(struct entity* en, int yspeed){
    en->dy = JUMP_VELOCITY;
    en->behavior = JUMPING;
}

struct tile* updateCharacter(struct character* character, struct tile** tiles, struct entityList* fireballs,
ALLEGRO_SAMPLE** samples, ALLEGRO_AUDIO_STREAM** tracks){
    struct tile* rValue = NULL; // Valor de retorno
    character->invincibility -= 1;
    if(character->invincibility == 0) {
        al_detach_audio_stream(tracks[STAR_TRACK]);
        character->star = 0;
        al_attach_audio_stream_to_mixer(tracks[GAME_TRACK], al_get_default_mixer());
    }

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
                jump(character->self, JUMP_VELOCITY);
                al_play_sample(samples[JUMP_SAMPLE], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
            else if (key[ALLEGRO_KEY_LEFT])
                walk(character->self, CHARACTER_SPEED, LEFT);
            else if (key[ALLEGRO_KEY_RIGHT])
                walk(character->self, CHARACTER_SPEED, RIGHT);
            else
                character->self->dx = 0; // Para de acelerar
            
            tileUpCollision(character->self, tiles);
            // Se colidiu lateralmente impede que ande
            if(tileLeftCollision(character->self, tiles) || tileRightCollision(character->self, tiles))
                character->self->behavior = IDLE;

            break;
        case JUMPING:
            if(key[ALLEGRO_KEY_LEFT])
                move(character->self, CHARACTER_SPEED, LEFT); 
            else if(key[ALLEGRO_KEY_RIGHT])
                move(character->self, CHARACTER_SPEED, RIGHT);
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
            if (key[ALLEGRO_KEY_UP]){ // Pula
                jump(character->self, JUMP_VELOCITY);
                al_play_sample(samples[JUMP_SAMPLE], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
            else if (key[ALLEGRO_KEY_LEFT])
                walk(character->self, CHARACTER_SPEED, LEFT);
            else if (key[ALLEGRO_KEY_RIGHT])
                walk(character->self, CHARACTER_SPEED, RIGHT);
            else { // Para
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
    // Se estiver sob efeito da estrela corre mais rapido
    character->self->x += character->star ? 1.5*character->self->dx : character->self->dx;

    return rValue;
}

struct entity* tileInteract(struct character* character, struct tile* t, ALLEGRO_SAMPLE** samples, int* score){
    if(t == NULL) return NULL;

    struct entity* newEn = NULL;
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

// Da um poder ao personagem, baseado no powerType
void givePower(struct character* character, int powerType, ALLEGRO_SAMPLE** samples,
ALLEGRO_AUDIO_STREAM** tracks, int* score){
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
                al_play_sample(samples[POWERUP_SAMPLE], 1.0, 1.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
            else
                *score += BUFF_SCORE;
            break;
        case STAR_POWER:
            character->invincibility = INVINCIBILTY;
            character->star = 1;
            al_play_sample(samples[POWERUP_SAMPLE], 1.0, 1.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            al_detach_audio_stream(tracks[GAME_TRACK]);
            al_attach_audio_stream_to_mixer(tracks[STAR_TRACK], al_get_default_mixer());
            break;
        
        case NO_POWER:
            character->power = NO_POWER;
            character->self->w = SMALL_WIDTH;
            character->self->h = SMALL_HEIGHT;
            character->self->anim->sprite = SMALL_CHAR_SPRITE;
            character->self->behavior = JUMPING;
            al_play_sample(samples[UNPOWER_SAMPLE], 1.0, 1.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            break;
    }
}

// Esmaga uma entidade
void shatter(struct character* character, struct entityNode* current, struct entityList* entities,
ALLEGRO_SAMPLE** samples, int* score){
    struct entity* newEn = NULL;
    switch(current->en->type){
        case SHELL: // Se for casco o para
            current->en->behavior = IDLE;
            current->en->dx = 0;
            character->self->dy = LOW_BOUNCE;
            break;
        case TURTLE: // Se for tartaruga cria um casco
            newEn = newEntity(
                SHELL, current->en->x, current->en->y,
                current->en->w, current->en->h / 2, current->en->dir,
                newAnimation(SHELL_SPRITE, 0, FRAMES_N-1, WALK_DURATION),
                INFINITE
            );
            newEn->dy += GRAVITY;
            insertEntity(entities, newEn);
        default: // Mata a entidade
            removeEntity(current->id, entities);
            *score += KILL_SCORE;
            character->self->dy = HIGH_BOUNCE;
            al_play_sample(samples[BOUNCE_SAMPLE], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            break;
    }
}

// Realiza a interação entre o personagem e uma entidade qualquer
// Se acabou o jogo retorna 1, do contrário 0
int interact(struct character* character, struct entityList* entities, struct entityNode* current,
ALLEGRO_SAMPLE** samples, ALLEGRO_AUDIO_STREAM** tracks, int* score){
    
    switch(current->en->type){
        case CHECKPOINT: // Se for o checkpoint, termina o jogo
            al_play_sample(samples[CHECKPOINT_SAMPLE], 20.0, 1.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
            return 1;
        case MUSHROOM: case FLOWER: case STAR:// Se for um power up, da poder ao personagem
            givePower(character, current->en->type, samples, tracks, score);
            removeEntity(current->id, entities);
            break;
        default: // Se não for nenhum dos dois, é um inimigo
            if(entityDownCollision(character->self, current->en)){
                // Se pulou em cima esmaga a entidade
                shatter(character, current, entities, samples, score);
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
                givePower(character, NO_POWER, samples, tracks, NULL);
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
struct entityList* fireballs, ALLEGRO_SAMPLE** samples, ALLEGRO_AUDIO_STREAM** tracks,  int* score){
    // Se o personagem cair nos limites inferiores do mapa, o mata
    if(character->self->y + character->self->h > VIRTUAL_HEIGHT)
        return 1;

    fireballsUpdate(fireballs, tiles);

    struct entityNode* current = NULL;
    struct entityNode* next = entities->start; 

    // Navega por todas a entidades e reaje conforme o tipo das entidades
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
            if(current->en->type == SHELL){
                // Se um casco em movimento atingir alguma entidade, mata a entiade
                int id = listIterate(shellHit, entities, current->en);
                if(id != -1) {
                    removeEntity(id, entities);
                    al_play_sample(samples[HIT_SAMPLE], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            }
            if(entityCollision(character->self, current->en))
                return interact(character, entities, current, samples, tracks, score);
        }        
    }

    return 0;
}