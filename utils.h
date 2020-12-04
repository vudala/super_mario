#ifndef UTILS_INCLUDED
#define UTILS_INCLUDED

// Aloca um espaço de memória para uma matriz
void** allocateMatrix(int size, long int width, long int height);

// Certifica que o espaço de memória foi alocado com sucesso
void mustAllocate(void* ptr, char* str);

#endif