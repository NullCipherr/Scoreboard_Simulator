#include "barramento.h"

#include "defs.h"

global char *_memoria;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                          PUBLIC FUNCTIONS                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void
barramento_inicializar(char *memoria) {
  _memoria = memoria;
}

uint32_t
barramento_buscar_instrucao(int PC) {
  uint32_t instrucao = *((int *) &_memoria[PC]);

  return instrucao;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                          PRIVATE FUNCTIONS                              *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
