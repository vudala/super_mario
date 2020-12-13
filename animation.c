#include "animation.h"
#include "entity.h"
#include "utils.h"
#include "level.h"
#include "game.h"
#include "sound.h"
#include "stdio.h"

#include <math.h>

// Carrega uma sprite de n frames
ALLEGRO_BITMAP** loadSprite(char* path, int frames_n){
    ALLEGRO_BITMAP* mainBitmap = al_load_bitmap(path);
   
    int width = al_get_bitmap_width(mainBitmap);
    int height = al_get_bitmap_height(mainBitmap);
    int frameWidth = width / frames_n;

    ALLEGRO_BITMAP** sprites = calloc(frames_n, sizeof(ALLEGRO_BITMAP*));
    mustAllocate(sprites, "sprites");

    ALLEGRO_BITMAP* subBitmap = NULL;

    for(int i = 0; i < frames_n; i++){
        subBitmap = al_create_sub_bitmap(mainBitmap, i * frameWidth, 0, frameWidth, height);
        mustAllocate(subBitmap, path);
        sprites[i] = al_clone_bitmap(subBitmap);
    }

    al_destroy_bitmap(mainBitmap);
    
    return sprites;
}

ALLEGRO_BITMAP** loadScreens(){
    ALLEGRO_BITMAP** screens = calloc(SCREENS_N, sizeof(ALLEGRO_BITMAP*));
    mustAllocate(screens, "screens");

    screens[START_SCREEN] = al_load_bitmap("resources/sprites/screens/start.png");
    mustAllocate(screens[START_SCREEN], "start screen");

    screens[HELP_SCREEN] = al_load_bitmap("resources/sprites/screens/help.png");
    mustAllocate(screens[HELP_SCREEN],"help screen");
    
    screens[SCORES_SCREEN] = al_load_bitmap("resources/sprites/screens/scores.png");
    mustAllocate(screens[SCORES_SCREEN],"scores screen");

    screens[BACKGROUND_SCREEN] = al_load_bitmap("resources/sprites/screens/background.png");
    mustAllocate(screens[BACKGROUND_SCREEN],"background screen");

    return screens;
}

ALLEGRO_BITMAP** loadTileSprites(){
    ALLEGRO_BITMAP** sprites = calloc(TILE_SPRITES_N, sizeof(ALLEGRO_BITMAP*));
    mustAllocate(sprites, "sprites");

    sprites = loadSprite("resources/sprites/tiles/tiles.png", TILE_SPRITES_N);

    return sprites;
}

ALLEGRO_BITMAP*** loadEntitySprites(){
    ALLEGRO_BITMAP*** sprites = calloc(ENTITY_SPRITES_N, sizeof(ALLEGRO_BITMAP**));
    mustAllocate(sprites, "sprites");
    // DA PRA MELHORA ISSO AQUI --------------------------------------------------
    sprites[CHAR_SPRITE] = loadSprite("resources/sprites/entities/mario.png" , FRAMES_N);
    sprites[SMALL_CHAR_SPRITE] = loadSprite("resources/sprites/entities/mario_small.png", FRAMES_N);
    sprites[CHAR_FLOWER_SPRITE] = loadSprite("resources/sprites/entities/mario_flower.png", FRAMES_N);
    sprites[GOOMBA_SPRITE] = loadSprite("resources/sprites/entities/goomba.png", FRAMES_N);
    sprites[TURTLE_SPRITE] = loadSprite("resources/sprites/entities/turtle.png", FRAMES_N);
    sprites[FLOWER_SPRITE] = loadSprite("resources/sprites/entities/flower.png", FRAMES_N);
    sprites[STAR_SPRITE] = loadSprite("resources/sprites/entities/star.png", FRAMES_N);
    sprites[SHELL_SPRITE] = loadSprite("resources/sprites/entities/shell.png", FRAMES_N);
    sprites[MUSHROOM_SPRITE] = loadSprite("resources/sprites/entities/mushroom.png", FRAMES_N);
    sprites[COIN_SPRITE] = loadSprite("resources/sprites/entities/coin.png", FRAMES_N);
    sprites[FIREBALL_SPRITE] = loadSprite("resources/sprites/entities/fireball.png", FRAMES_N);
    sprites[CHECKPOINT_SPRITE] = loadSprite("resources/sprites/entities/checkpoint.png", FRAMES_N);

    return sprites;
}
 
int entitySpriteID(char type){
    switch(type){
        case MAIN_CHARACTER: return CHAR_SPRITE; break;
        case MAIN_SMALL: return SMALL_CHAR_SPRITE; break;
        case MAIN_FLOWER: return CHAR_FLOWER_SPRITE; break;
        case GOOMBA: return GOOMBA_SPRITE; break;
        case TURTLE: return TURTLE_SPRITE; break;
        case FLOWER: return FLOWER_SPRITE; break;
        case STAR: return STAR_SPRITE; break;
        case SHELL: return SHELL_SPRITE; break;
        case MUSHROOM: return MUSHROOM_SPRITE; break;
        case COIN: return COIN_SPRITE; break;
        case FIREBALL: return FIREBALL_SPRITE; break;
        case CHECKPOINT: return CHECKPOINT_SPRITE; break;
    }

    return MUSHROOM_SPRITE;
}

struct animation* newAnimation(int sprite, int start, int end, int duration){
    struct animation* a = calloc(1, sizeof(struct animation));
    mustAllocate(a, "animation");

    a->sprite = sprite;
    a->duration = duration;
    a->currentClock = duration;
    a->start = start;
    a->end = end;
    a->currentFrame = 0;
    a->reset = 0;

    return a;
}

// Atualiza o fram atual de uma animação
void updateAnimationFrame(struct animation* anim){
    if(anim->currentClock == 0){
        // Se chegou ao fim começa a voltar
        if(anim->currentFrame >= anim->end) anim->reset = 1;
        // Se voltou ao começo começa a avançar novamente
        if(anim->currentFrame == anim->start) anim->reset = 0;
        
        anim->currentFrame += anim->reset ? -1 : 1;

        anim->currentClock = anim->duration;
    } else anim->currentClock -= 1;
}

void drawEntity(struct entity* en, int* offset, ALLEGRO_BITMAP*** sprites, ALLEGRO_COLOR color){
    // Se estiver virado pra direita desenha normalmente, se não irá desenhar ao contrário
    int xScale = en->dir ? 1 : -1;
    // Deslocamento do desenho
    int dw = en->dir ? 0 : en->w;

    // Seleciona qual frame deve desenhar
    ALLEGRO_BITMAP* frame = sprites[en->anim->sprite][IDLE_FRAME];
    switch(en->behavior){
        case WALKING:
            updateAnimationFrame(en->anim);
            frame = sprites[en->anim->sprite][en->anim->currentFrame];
            break;
        case JUMPING: case BOUNCING:
            frame = sprites[en->anim->sprite][JUMP_FRAME];
            break;
    }

    al_draw_tinted_scaled_bitmap(
        frame, color, 0, 0, en->w, en->h,
        floor(*offset + en->x) + dw, floor(en->y),
        en->w * xScale, en->h, 0
    );
}

void drawEntities(struct entityList* list, int* offset, ALLEGRO_BITMAP*** sprites, ALLEGRO_COLOR color){
    struct entityNode* next = list->start;
    while(next != NULL){
        drawEntity(next->en, offset, sprites, color);
        next = next->next;
    }
}

int tileSpriteID(char type){
    switch(type){
        case EMPTY_BLOCK: return EMPTY_BLOCK_SPRITE; break;
        case BRICK_BLOCK: return BRICK_BLOCK_SPRITE; break;
        case BRICK_BODY_BLOCK: return BRICK_BODY_SPRITE; break;
        case HARD_BRICK_BLOCK: return HARD_BRICK_BLOCK_SPRITE; break;
        case PIPE_BLOCK: return PIPE_BLOCK_SPRITE; break;
        case PIPE_BODY_BLOCK: return PIPE_BODY_SPRITE; break;
        case COIN_BLOCK: return COIN_BLOCK_SPRITE; break;
        case STAR_BLOCK: return STAR_BLOCK_SPRITE; break;
        case MUSHROOM_BLOCK: return MUSHROOM_BLOCK_SPRITE; break;
        case FLOWER_BLOCK: return FLOWER_BLOCK_SPRITE; break;
    }
    
    return EMPTY_BLOCK_SPRITE;
}

void drawTiles(struct tile** tiles, ALLEGRO_BITMAP** sprites, int* offset){
    int whichSprite = 0;
    ALLEGRO_COLOR color;
    for(int y = 0; y < MAP_HEIGHT; y++){
        for(int x = 0; x < MAP_WIDTH; x++){
            if(tiles[y][x].active){
                whichSprite = tileSpriteID(tiles[y][x].type);
                // Se tiver conteudo, pinta de branco para nao inteferir
                color = al_map_rgb(RGB_MAX, RGB_MAX, RGB_MAX); 

                if(!tiles[y][x].content) // Se não tiver conteúdo pinta de cinza
                    color = al_map_rgb(105,105,105);

                al_draw_tinted_bitmap(
                    sprites[whichSprite],
                    color, 
                    tiles[y][x].x + *offset,
                    tiles[y][x].y, 0
                );
            }
        }
    }
}

int drawEnd(ALLEGRO_BITMAP** screens, ALLEGRO_SAMPLE** samples, ALLEGRO_AUDIO_STREAM** tracks, int* score){
    // Começa a soundtrack do fim
    al_attach_audio_stream_to_mixer(tracks[END_TRACK], al_get_default_mixer());

    bool done = false;
    bool redraw = true;
    int newState = DESTROY;

    // Cria estrututras a serem usadas pela tela de pontuações
    int* scores = getScores(score); // Os scores
    char* aux = malloc(sizeof(char) * 40); // A string auxiliar para desenhar
    mustAllocate(aux, "aux"); 
    struct animation* anim = newAnimation(-1, 0, 2, 30); // A animação do fim
    // As sprites da animação
    ALLEGRO_BITMAP** sprite = loadSprite("resources/sprites/screens/ending.png", END_SPRITES_N); 
    ALLEGRO_COLOR color = al_map_rgb(RGB_MAX, RGB_MAX, RGB_MAX); // A cor das letras

    for(;;){
        al_wait_for_event(queue, &event);
        switch(event.type){
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                switch(event.keyboard.keycode){
                    case ALLEGRO_KEY_ESCAPE: // Se apertou esc sai da tela ou do jogo
                        done = true;
                        break;
                    case ALLEGRO_KEY_ENTER: // Se estiver na teça de pontuação e aperta enter, joga novamente
                        al_play_sample(
                            samples[SELECT_SAMPLE], 1.0, 1.0, 1.0,
                            ALLEGRO_PLAYMODE_ONCE, NULL
                        );
                        newState = PLAY;
                        done = true;
                        break;
                }
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done) break;

        if(redraw && al_is_event_queue_empty(queue)){
            // Desenha o fundo
            updateAnimationFrame(anim);
            al_draw_bitmap(screens[BACKGROUND_SCREEN], 0, 0, 0);
            al_draw_bitmap(sprite[anim->currentFrame], 0, 0, 0);
            
            // Desenha a tela
            al_draw_bitmap(screens[SCORES_SCREEN], 0, 0, 0);

            // Desenha as pontuações
            sprintf(aux, "Sua pontuação é: %d", *score);
            al_draw_text(font, color, 600, 100, 0 , aux);
            for(int i = 0; i < TOP_SCORE_N; i++){
                sprintf(aux, "%2d %9d", i+1, scores[i]); 
                al_draw_text(font, color, 600, 150 + i * 40, 0 , aux);
            }

            al_flip_display();
            redraw = false;
        }
    }

    // Destroi as estruturas criadas para animar o final
    free(scores);
    scores = NULL;
    free(aux);
    aux = NULL;

    for(int i = 0; i < END_SPRITES_N; i++)
        al_destroy_bitmap(sprite[i]);
    free(sprite);
    sprite = NULL;

    free(anim);
    anim = NULL;

    // Reseta todas as teclas apertadas
    for(int i = 0; i < ALLEGRO_KEY_MAX; i++) key[i] = 0;

    al_detach_audio_stream(tracks[END_TRACK]);
    return newState;
}

int drawScreen(ALLEGRO_BITMAP** screens, int which, ALLEGRO_SAMPLE** samples, ALLEGRO_AUDIO_STREAM** tracks){
    bool done = false;
    bool redraw = true;
    int newState = DESTROY;

    for(;;){
        al_wait_for_event(queue, &event);
        switch(event.type){
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                switch(event.keyboard.keycode){
                    case ALLEGRO_KEY_ESCAPE: // Se apertou esc sai da tela
                        al_play_sample(
                            samples[SELECT_SAMPLE], 1.0, 1.0, 1.0,
                            ALLEGRO_PLAYMODE_ONCE, NULL
                        );
                        done = true;
                        break;
                    case ALLEGRO_KEY_H: // Se estiver no menu e apertaR H, abre a tela de ajuda
                        if(which == START_SCREEN){ 
                            al_play_sample(
                                samples[SELECT_SAMPLE], 1.0, 1.0, 1.0,
                                ALLEGRO_PLAYMODE_ONCE, NULL
                            );
                            drawScreen(screens, HELP_SCREEN, samples, tracks);
                        }
                        break;
                    case ALLEGRO_KEY_UP: // Se estiver no menu e apertar UP, começa o jogo
                        if(which == START_SCREEN){ 
                            al_play_sample(
                                samples[SELECT_SAMPLE], 1.0, 1.0, 1.0,
                                ALLEGRO_PLAYMODE_ONCE, NULL
                            );
                            newState = PLAY;
                            done = true;
                        }
                        break;
                }
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done) break;

        if(redraw && al_is_event_queue_empty(queue)) {
            // Desenha a tela
            al_draw_bitmap(screens[which], 0, 0, 0);
            al_flip_display();
            redraw = false;
        }
    }
    
    // Reseta todas as teclas apertadas
    for(int i = 0; i < ALLEGRO_KEY_MAX; i++) key[i] = 0;

    return newState;
}