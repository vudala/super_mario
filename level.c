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

struct tile** load_level(char* levelPath, struct tile** tiles, struct entity* enemies){
    FILE* file = fopen(levelPath, "r");
    mustAllocate(file, levelPath);
    struct tile** t = (struct tile**) allocateMatrix(sizeof(struct tile), MAP_WIDTH, MAP_HEIGHT);

    struct tile* newT = NULL;
    //struct entity* newE = NULL;
    char c;
    int i = 0, j = 0;
    while((c = fgetc(file)) != EOF){
        if(j >= MAP_WIDTH){
            fgetc(file);
            i++;
            j = 0;
        }
        switch(c){
            case MAIN_CHARACTER:
                newT = newTile( j * TILE_WIDTH, i * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, c != EMPTY_BLOCK ? 1 : 0, c);
                t[i][j] = *newT;
                free(newT);
                break;
            case GOOMBA:
                newT = newTile( j * TILE_WIDTH, i * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, c != EMPTY_BLOCK ? 1 : 0, c);
                t[i][j] = *newT;
                free(newT);
                break;
            case TURTLE:
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