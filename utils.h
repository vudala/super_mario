#ifndef UTILS_INCLUDED
#define UTILS_INCLUDED

#include <stdbool.h>

// Aloca um espaço de memória para uma matriz
void** allocateMatrix(int size, long int width, long int height);

// Certifica que o espaço de memória foi alocado com sucesso
void mustAllocate(void* ptr, char* str);

// Certifica que a estrutura foi inicializada
void mustInit(bool test, const char *description);

#endif