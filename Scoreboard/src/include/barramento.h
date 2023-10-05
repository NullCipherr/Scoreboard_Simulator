#pragma once

#include <stdint.h>

void     barramento_inicializar(char *memoria);
uint32_t barramento_buscar_instrucao(int PC);
void     barramento_escrever_dado(int endereco, int dado);
