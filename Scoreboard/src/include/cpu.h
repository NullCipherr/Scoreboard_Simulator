#pragma once

#include <stdint.h>

typedef int               Registrador;
typedef struct _cpu_specs CPU_Specs;
typedef Registrador       Banco_Registradores[32];
typedef struct _uf        UF;
typedef struct _banco_uf  Banco_UF;

struct _uf {
  uint8_t     operacao; // OpCode
  Registrador Fi;       // Destino
  Registrador Fj, Fk;   // Valores
  int         Qj, Qk;   // Unidades produtoras
  int         Rj, Rk;   // Flags de disponibilidade
  int         busy;     // Flag de ocupação
};

struct _banco_uf {
  UF *add;
  UF *mul;
  UF *inteiro;
};

struct _cpu_specs {
  uint32_t uf_add;
  uint32_t uf_mul;
  uint32_t uf_int;

  uint32_t clock_add;
  uint32_t clock_addi;
  uint32_t clock_sub;
  uint32_t clock_subi;
  uint32_t clock_mul;
  uint32_t clock_div;
  uint32_t clock_and;
  uint32_t clock_or;
  uint32_t clock_not;
  uint32_t clock_blt;
  uint32_t clock_bgt;
  uint32_t clock_beq;
  uint32_t clock_bne;
  uint32_t clock_jump;
  uint32_t clock_load;
  uint32_t clock_store;
  uint32_t clock_exit;

  uint32_t qtd_instrucoes;
};

void scoreboard_inicializar(CPU_Specs *cpu_specs);
void rodar_programa(char *nome_saida);
