#include "utils.h"

#include <stdlib.h>
#include <stdio.h>

void** allocateMatrix(int size, long int width, long int height){
    void** m;

    m = calloc(height, size);
    mustAllocate(m, "matrix");
    m[0] = calloc(height * width, size);
    mustAllocate(m[0], "matrix");
    for (int i=1; i < height; i++)
        m[i] = m[0] + i * width * size;

    return m;
}

void mustInit(bool test, const char *description){
    if(test) return;

    printf("Couldn't initialize %s\n", description);
    exit(1);
}

void mustAllocate(void* ptr, char* str){
    if(ptr == NULL){
        fprintf(stderr, "Error allocating memory to %s\n", str);
        exit(1);
    }
}

// Funcao utilizada para ordenar o vetor de scores em ordem decrescente
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)b - *(int*)a);
}

int* getScores(int* currScore){
    FILE* file = fopen("resources/database/top_scores.txt", "r");
    mustAllocate(file, "score file");

    int* scores = calloc(TOP_SCORE_N, sizeof(int));
    mustAllocate(scores, "scores");
    
    // Armazena uma string de até 9 chars, ou seja, armazena um decimal de até 9 digitos
    char* s = malloc(9); 
    for(int i = 0; fgets(s, 9, file) && i < TOP_SCORE_N; i++)
        scores[i] = atoi(s);

    // Organiza os scores em ordem decrescente
    if(scores[TOP_SCORE_N-1] < *currScore) scores[TOP_SCORE_N-1] = *currScore;
    qsort(scores, TOP_SCORE_N, sizeof(int), cmpfunc);

    // Sobrescreve os scores antigos com o primeiro score
    file = freopen("resources/database/top_scores.txt", "w", file);
    mustAllocate(file, "score file");
    fprintf(file, "%d\n", scores[0]);

    // Escreve os scores atualizados
    file = freopen("resources/database/top_scores.txt", "a", file);
    mustAllocate(file, "score file");
    for(int i = 1; i < TOP_SCORE_N; i++)
        fprintf(file, "%d\n", scores[i]);

    fclose(file);

    return scores;
}