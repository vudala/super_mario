#ifndef UTILS_INCLUDED
#define UTILS_INCLUDED

#include <stdbool.h>

#define TOP_SCORE_N 10 // Quantos scores são guardados no top score

// Aloca um espaço de memória para uma matriz
void** allocateMatrix(int size, long int width, long int height);

// Certifica que o espaço de memória foi alocado com sucesso
void mustAllocate(void* ptr, char* str);

// Certifica que a estrutura foi inicializada
void mustInit(bool test, const char *description);

// Captura os scores totais e os classifica junto com o seu
int* getScores(int* currScore);

#endif