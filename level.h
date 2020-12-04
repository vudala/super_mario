#ifndef LEVEL_INCLUDED
#define LEVEL_INCLUDED

struct entity;

typedef enum {
    EMPTY_BLOCK = ' ',
    BRICK_BLOCK = 'b',
    HARD_BRICK_BLOCK = 'h',
    PIPE_BLOCK = 'p',
    PIPE_TOP_BLOCK = 'P',
    COIN_BLOCK = 'c',
    STAR_BLOCK = 's',
    MUSHROOM_BLOCK = 'm',
    FLOWER_BLOCK = 'f'
} TilesType;

struct tile {
    int x;
    int y;
    int w;
    int h;
    int active;
    int type;
    int highlight;
};

struct tile** load_level(char* levelPath, struct tile** tiles);

#endif