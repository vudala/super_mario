#include "utils.h"

#include <stdlib.h>
#include <stdio.h>

void** allocateMatrix(int size, long int width, long int height){
    void** m;

    m = calloc(height, size);
    m[0] = calloc(height * width, size);
    for (int i=1; i < height; i++)
        m[i] = m[0] + i * width * size;

    return m;
}

void mustAllocate(void* ptr, char* str){
    if(ptr == NULL){
        fprintf(stderr, "Error allocating memory to %s\n", str);
        exit(1);
    }
}