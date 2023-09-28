#pragma once

#include <stdint.h>

typedef enum { R, I, J } tipo;

void decodificar(uint32_t instrucao);
