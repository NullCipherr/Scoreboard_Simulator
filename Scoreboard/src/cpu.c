#include "cpu.h"

#include "barramento.h"
#include "defs.h"
#include "hashtables.h"
#include "memoria.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _lista_instrucoes Lista_Instrucoes;
typedef struct _instrucao_no     Instrucao_No;

struct _instrucao_no {
  uint32_t      instrucao;
  int           clocks_restantes;
  tipo          tipo;
  Instrucao_No *proximo;
};

struct _lista_instrucoes {
  Instrucao_No *cabeca;
  Instrucao_No *fim;
};

global Lista_Instrucoes    lista_emissao    = { 0 };
global Lista_Instrucoes    lista_leitura    = { 0 };
global Lista_Instrucoes    lista_executando = { 0 };
global Lista_Instrucoes    lista_escrita    = { 0 };
global uint32_t            ClockMap[17]     = { 0 };
global uint32_t            add_usados = 0, mul_usados = 0, int_usados = 0;
global int                 PC = 100;
global CPU_Specs           _cpu_specs;
global Banco_Registradores banco_registradores = { 0 };

internal void adicionar_instrucao(uint32_t instrucao);
internal void printar_scoreboard(void);
internal void emitir(void);
internal void checar_waw(void);
internal void leitura_operandos(void);
internal void checar_raw(void);
internal void executar(void);
internal void escrever(void);
internal void checar_war(void);
internal void mandar_ler(Instrucao_No *instrucao);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                          PUBLIC FUNCTIONS                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void
scoreboard_inicializar(CPU_Specs *cpu_specs) {
  _cpu_specs   = *cpu_specs;
  ClockMap[0]  = _cpu_specs.clock_add;
  ClockMap[1]  = _cpu_specs.clock_addi;
  ClockMap[2]  = _cpu_specs.clock_sub;
  ClockMap[3]  = _cpu_specs.clock_subi;
  ClockMap[4]  = _cpu_specs.clock_mul;
  ClockMap[5]  = _cpu_specs.clock_div;
  ClockMap[6]  = _cpu_specs.clock_and;
  ClockMap[7]  = _cpu_specs.clock_or;
  ClockMap[8]  = _cpu_specs.clock_not;
  ClockMap[9]  = _cpu_specs.clock_blt;
  ClockMap[10] = _cpu_specs.clock_bgt;
  ClockMap[11] = _cpu_specs.clock_beq;
  ClockMap[12] = _cpu_specs.clock_bne;
  ClockMap[13] = _cpu_specs.clock_jump;
  ClockMap[14] = _cpu_specs.clock_load;
  ClockMap[15] = _cpu_specs.clock_store;
  ClockMap[16] = _cpu_specs.clock_exit;
}

void
rodar_programa(char *nome_saida, CPU_Specs *cpu_specs) {
  int rodando = 1;
  while (rodando) {                                       // 0x10 = EXIT
    uint32_t instrucao = barramento_buscar_instrucao(PC); // Busca inicial
    adicionar_instrucao(instrucao);                       // Parte da busca
    emitir();                                             // Emissão
    leitura_operandos(); // Leitura dos operandos
    executar();          // Execução
    escrever();          // Escrita
    PC        += 4;      // Incrementa o PC
    instrucao  = barramento_buscar_instrucao(PC);
  }
}

internal void
leitura_operandos(void) {
  Instrucao_No *instrucao = lista_leitura.cabeca;

  while (instrucao) {
    uint8_t  opcode = instrucao->instrucao >> 26;
    uint8_t  rd = 0, rs = 0, rt = 0;
    uint16_t imm = 0, extra = 0;
    uint32_t end = 0;

    switch (OpCodeTipo[opcode]) {
    case R:
      rd    = (instrucao->instrucao >> 11) & 0x1F;
      rs    = (instrucao->instrucao >> 21) & 0x1F;
      rt    = (instrucao->instrucao >> 16) & 0x1F;
      extra = instrucao->instrucao & 0x7FF;
      break;
    case I:
      rt  = (instrucao->instrucao >> 16) & 0x1F;
      rs  = (instrucao->instrucao >> 21) & 0x1F;
      imm = instrucao->instrucao & 0xFFFF;
      break;
    case J: end = instrucao->instrucao & 0x3FFFFFF; break;
    default: perror("Tipo de instrução não reconhecido"); exit(EXIT_FAILURE);
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                          PRIVATE FUNCTIONS                              *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal void
adicionar_instrucao(uint32_t instrucao) {
  Instrucao_No *no     = (Instrucao_No *) malloc(sizeof(Instrucao_No));
  no->instrucao        = instrucao;
  no->proximo          = NULL;
  no->clocks_restantes = ClockMap[instrucao >> 26];
  no->tipo             = OpCodeTipo[instrucao >> 26];

  if (lista_emissao.cabeca == NULL) {
    lista_emissao.cabeca = no;
    lista_emissao.fim    = no;
  } else {
    lista_emissao.fim->proximo = no;
    lista_emissao.fim          = no;
  }

  printf("Instrução adicionada: %u\n", instrucao);
}

internal void
printar_scoreboard(void) {
  printf("Scoreboard:\n");
}

internal void
emitir(void) {
  Instrucao_No *instrucao_emitida    = lista_emissao.cabeca;
  Instrucao_No *instrucao_executando = lista_executando.cabeca;

  int registradores_usados[32] = { 0 };

  while (instrucao_executando) {
    switch (instrucao_executando->tipo) {
    case R:
      registradores_usados[(instrucao_executando->instrucao >> 11) & 0x1F] = 1;
      break;
    case I:
      if ((instrucao_executando->instrucao >> 26) < 4) {
        registradores_usados[(instrucao_executando->instrucao >> 16) & 0x1F] =
            1;
      } else {
        registradores_usados[(instrucao_executando->instrucao >> 21) & 0x1F] =
            1;
      }
      break;
    case J: break;
    }

    instrucao_executando = instrucao_executando->proximo;
  }

  while (instrucao_emitida) {
    uint8_t opcode = instrucao_emitida->instrucao >> 26;

    // Checa se UF disponível
    // Tá feio mas acho que tá certo
    if (opcode < 4 && add_usados < _cpu_specs.uf_add) {
    } else if (opcode < 6 && mul_usados < _cpu_specs.uf_mul) {
    } else if (opcode < 16 && int_usados < _cpu_specs.uf_int) {
    } else {
      continue;
    }

    // Checando WAW
    switch (instrucao_emitida->tipo) {
    case R:
      if (!registradores_usados[(instrucao_emitida->instrucao >> 11) & 0x1F]) {
        mandar_ler(instrucao_emitida);
      }
      break;
    case I:
      if ((instrucao_emitida->instrucao >> 26) < 4) {
        if (!registradores_usados[(instrucao_emitida->instrucao >> 16) & 0x1F])
        {
          mandar_ler(instrucao_emitida);
        }
      } else {
        if (!registradores_usados[(instrucao_emitida->instrucao >> 21) & 0x1F])
        {
          mandar_ler(instrucao_emitida);
        }
      }
      break;
    case J: break;
    }

    instrucao_emitida = instrucao_emitida->proximo;
  }
}

internal void
escrever(void) {}

internal void
executar(void) {}

internal void
mandar_ler(Instrucao_No *instrucao) {
  Instrucao_No *emitida = lista_emissao.cabeca;

  while (emitida->proximo != instrucao) {
    emitida = emitida->proximo;
  }

  emitida->proximo = instrucao->proximo;

  if (lista_leitura.cabeca == NULL) {
    lista_leitura.cabeca = instrucao;
    lista_leitura.fim    = instrucao;
  } else {
    lista_leitura.fim->proximo = instrucao;
    lista_leitura.fim          = instrucao;
  }
}
