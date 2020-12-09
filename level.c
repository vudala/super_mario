#include "level.h"
#include "utils.h"
#include "entity.h"
#include "entity_list.h"
#include "animation.h"
#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct tile* newTile(int x, int y, int w, int h, int active, int type){
    struct tile* t = calloc(1, sizeof(struct tile));
    mustAllocate(t, "tile");

    t->x = x;
    t->y = y;
    t->w = w;
    t->h = h;
    t->active = active;
    t->type = type;
    t->content = type == COIN_BLOCK ? 3 : 1;

    return t;
}

struct tile* pointToTile(int x, int y, struct tile** tiles){
    if (x < 0 || y < 0 || x > MAP_WIDTH * TILE_WIDTH || y > MAP_HEIGHT * TILE_HEIGHT) return NULL;
    int i = floor(y / TILE_HEIGHT); // Linha da tile
    int j = floor(x / TILE_WIDTH); // Coluna da tile
    return &tiles[i][j];
}

int specialTileContent(char type){
    switch(type){
        case COIN_BLOCK: return COIN; break;
        case STAR_BLOCK: return STAR; break;
        case MUSHROOM_BLOCK: return MUSHROOM; break;
        case FLOWER_BLOCK: return FLOWER; break;
    }
    
    return -1;
}

// Verifica se uma entidade está dentro dos limites de largura do mapa
int withinMapWidth(struct entity* en){
    if(en->x < 0) return 0;
    if(en->x + en->w - 1  >= MAP_WIDTH * TILE_WIDTH) return 0;
    
    return 1;
}

// Verifica se uma entidade está dentro dos limites de altura do mapa
int withinMapHeight(struct entity* en){
    if(en->x < 0 || en->y < 0) return 0;
    if(en->y + en->h >= MAP_HEIGHT * TILE_HEIGHT) return 0;
    
    return 1;
}

struct tile* tileLeftCollision(struct entity* en, struct tile** tiles){
    // Se não estiver dentro dos limites
    if(!withinMapWidth(en)) return NULL;
    if(!withinMapHeight(en)) return NULL;

    if(en->dx < 0){
        struct tile* left1 = pointToTile(en->x - 1, en->y, tiles);
        struct tile* left2 = pointToTile(en->x - 1, en->y + en->h - 1, tiles);
        if(left1->active || left2->active){
            en->dx = 0;
            en->x = left1->x+left1->w;
            if(left1->active) return left1;
            return left2;
        }
    }
    
    return NULL;
}

struct tile* tileRightCollision(struct entity* en, struct tile** tiles){
    // Se não estiver dentro dos limites
    if(!withinMapWidth(en)) return NULL;
    if(!withinMapHeight(en)) return NULL;

    if(en->dx > 0){
         struct tile* right1 = pointToTile(en->x + en->w, en->y, tiles);
        struct tile* right2 = pointToTile(en->x + en->w, en->y+en->h - 1, tiles);
        if(right1->active || right2->active){
            en->dx = 0;
            en->x = right1->x - right1->w;
            if(right1->active) return right1;
            return right2;
        }
    }

    return NULL;
}


struct tile* tileUpCollision(struct entity* en, struct tile** tiles){
    // Se não estiver dentro dos limites
    if(!withinMapHeight(en)) return NULL;

    struct tile* up1 = pointToTile(en->x + 5, en->y, tiles);
    struct tile* up2 = pointToTile(en->x+en->w - 5, en->y, tiles);

    if(up1->active || up2->active){
        en->dy = 0;
        en->y = up1->y + up1->h;
        if(up1->active) return up1;
        return up2;
    }
    
    return NULL;
}

struct tile* tileDownCollision(struct entity* en, struct tile** tiles){
    // Se não estiver dentro dos limites
    if(!withinMapHeight(en)) return NULL;

    struct tile* down1 = pointToTile(en->x + 5, en->y + en->h, tiles);
    struct tile* down2 = pointToTile(en->x+en->w - 5, en->y + en->h, tiles);
    
    if(down1->active || down2->active){
        en->dy = 0;
        en->y = down1->y - en->h;
        
        if(down1->active) return down1;
        return down2;
    }
    
    return NULL;
}

struct tile** loadLevel(char* levelPath, struct entityList* l, ALLEGRO_BITMAP*** sprites){
    FILE* file = fopen(levelPath, "r");
    mustAllocate(file, levelPath);
    struct tile** t = (struct tile**) allocateMatrix(sizeof(struct tile), MAP_WIDTH, MAP_HEIGHT);
    mustAllocate(t, "tiles");

    struct tile* newT = NULL;
    int whichSprite = 0;
    int width, height;

    char c;
    int i = 0, j = 0;
    while((c = fgetc(file)) != EOF){
        switch(c){
            case '\n':  
                i++;
                j = 0;
                break;
            /* Se corresponder a alguma entidade, a adiciona e coloca o bloco vazio no mesmo local */
            case GOOMBA: case TURTLE: case FLOWER: case STAR:
            case SHELL: case MUSHROOM: case COIN: 
                whichSprite = entitySpriteID(c);
                width = al_get_bitmap_width(sprites[whichSprite][0]);
                height = al_get_bitmap_height(sprites[whichSprite][0]);
                insertEntity(l,
                    newEntity(c, j*TILE_WIDTH, i*TILE_HEIGHT, width, height, RIGHT, 
                        newAnimation(whichSprite), -1
                    )
                );
                newT = newTile( j * TILE_WIDTH, i * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, 0, EMPTY_BLOCK);
                t[i][j] = *newT;
                free(newT);
                j++;
                break;
            /* Se não for nenhuma entidade, adiciona um bloco no local */
            default: 
                newT = newTile(j * TILE_WIDTH, i * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT,
                                c != EMPTY_BLOCK ? 1 : 0, c
                            );
                            
                t[i][j] = *newT;
                free(newT);
                j++;
                break;
        }
        
    }

    fclose(file);

    return t;
}