#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "level.h"
#include "game.h"
#include "entity.h"
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

    return t;
}

int getTileV(char c){
    if(c == EMPTY_BLOCK) return EMPTY_BLOCK_V; 
    if(c == BRICK_BLOCK) return BRICK_BLOCK_V;
    if(c == HARD_BRICK_BLOCK) return HARD_BRICK_BLOCK_V;
    if(c == PIPE_BLOCK) return PIPE_BLOCK_V;
    if(c == PIPE_TOP_BLOCK) return PIPE_TOP_BLOCK_V;
    if(c == COIN_BLOCK) return COIN_BLOCK_V;
    if(c == STAR_BLOCK) return STAR_BLOCK_V;
    if(c == MUSHROOM_BLOCK) return MUSHROOM_BLOCK_V;
    if(c == FLOWER_BLOCK) return FLOWER_BLOCK_V;
    return EMPTY_BLOCK_V;
}


struct tile* pointToTile(int x, int y, struct tile** tiles){
    if (x < 0 || y < 0 || x > MAP_WIDTH * TILE_WIDTH || y > MAP_HEIGHT * TILE_HEIGHT) return NULL;
    int i = floor(y / TILE_HEIGHT); // Linha da tile
    int j = floor(x / TILE_WIDTH); // Coluna da tile
    return &tiles[i][j];
}

struct tile** load_level(char* levelPath){
    FILE* file = fopen(levelPath, "r");
    mustAllocate(file, levelPath);
    struct tile** t = (struct tile**) allocateMatrix(sizeof(struct tile), MAP_WIDTH, MAP_HEIGHT);
    mustAllocate(t, "tiles");

    struct tile* newT = NULL;

    char c;
    int i = 0, j = 0;
    while((c = fgetc(file)) != EOF){
        if(j >= MAP_WIDTH){
            fgetc(file);
            i++;
            j = 0;
        }
        switch(c){
            case MAIN_CHARACTER: case GOOMBA: case TURTLE:
                newT = newTile( j * TILE_WIDTH, i * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, c != EMPTY_BLOCK ? 1 : 0, c);
                t[i][j] = *newT;
                free(newT);
                break;
            case '\n':
                break;
            default:
                newT = newTile( j * TILE_WIDTH, i * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, c != EMPTY_BLOCK ? 1 : 0, c);
                t[i][j] = *newT;
                free(newT);
                break;
        }
        
        j++;
    }

    fclose(file);

    return t;
}