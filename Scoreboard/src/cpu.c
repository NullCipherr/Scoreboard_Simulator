#include "cpu.h"

#include "barramento.h"
#include "defs.h"
#include "hashtables.h"
#include "memoria.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _lista_instrucoes   Lista_Instrucoes;
typedef struct _instrucao_no       Instrucao_No;
typedef struct _status_registrador Status_Registrador;
typedef struct _status_instrucoes  Status_Instrucoes;

typedef enum _tipo_uf { add, mul, inteiro } Tipo_UF;

struct _instrucao_no {
  uint32_t      instrucao;
  int           clocks_restantes;
  tipo          tipo;
  int           uf;
  Instrucao_No *proximo;
};

struct _lista_instrucoes {
  Instrucao_No *cabeca;
  Instrucao_No *fim;
};

struct _status_registrador {
  Tipo_UF uf;
  int     pos;
};

struct _status_instrucoes {
  char *instrucao;
  char  busca;
  char  emissao;
  char  leitura;
  char  execucao;
  char  escrita;
};

global Lista_Instrucoes    lista_emissao    = { 0 };
global Lista_Instrucoes    lista_leitura    = { 0 };
global Lista_Instrucoes    lista_executando = { 0 };
global Lista_Instrucoes    lista_escrita    = { 0 };
global uint32_t            ClockMap[17]     = { 0 };
global uint32_t            add_usados = 0, mul_usados = 0, int_usados = 0;
global int                 PC                     = 100;
global CPU_Specs           _cpu_specs             = { 0 };
global Banco_Registradores banco_registradores    = { 0 };
global Banco_UF            banco_uf               = { 0 };
global int                 rodando                = 1;
global Status_Registrador  status_registrador[32] = { 0 };
global Status_Instrucoes  *status_instrucoes      = NULL;

internal void adicionar_instrucao(uint32_t instrucao);
internal void printar_scoreboard(void);
internal void emitir(void);
internal void leitura_operandos(void);
internal void executar(void);
internal void escrever(void);
internal void mandar_ler(Instrucao_No *instrucao);
internal void mandar_executar(Instrucao_No *instrucao);
internal void mandar_escrever(Instrucao_No *instrucao);
internal void printar_ufs(void);
internal void printar_status_registradores(void);
internal void printar_instrucoes(void);

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

  banco_uf.add     = (UF *) malloc(sizeof(UF) * _cpu_specs.uf_add);
  banco_uf.mul     = (UF *) malloc(sizeof(UF) * _cpu_specs.uf_mul);
  banco_uf.inteiro = (UF *) malloc(sizeof(UF) * _cpu_specs.uf_int);

  status_instrucoes = (Status_Instrucoes *) malloc(sizeof(Status_Instrucoes)
                                                   * _cpu_specs.qtd_instrucoes);
}

void
rodar_programa(char *nome_saida) {
  uint32_t instrucao;

  while (rodando) {                              // 0x10 = EXIT
    instrucao = barramento_buscar_instrucao(PC); // Busca inicial
    escrever();                                  // Escrita
    executar();                                  // Execução
    leitura_operandos();                         // Leitura dos operandos
                                                 // Busca e emite no mesmo clock
    if (instrucao) {
      adicionar_instrucao(instrucao);            // Parte da busca
    }
    emitir();                                    // Emissão
    // TODO: Mudar local do PC
    PC += 4; // Incrementa o PC
    printar_scoreboard();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                          PRIVATE FUNCTIONS                              *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal void
leitura_operandos(void) {
  // Encontra a instrução que está sendo executada
  UF *add     = banco_uf.add;
  UF *mul     = banco_uf.mul;
  UF *inteiro = banco_uf.inteiro;

  Banco_Registradores registradores_usados = { 0 };

  for (uint i = 0; i < _cpu_specs.uf_add; i++) {
    if (add->Fi > -1) {
      registradores_usados[add->Fi] = 1;
    }
    add++;
  }

  for (uint i = 0; i < _cpu_specs.uf_mul; i++) {
    if (mul->Fi > -1) {
      registradores_usados[mul->Fi] = 1;
    }
    mul++;
  }

  for (uint i = 0; i < _cpu_specs.uf_int; i++) {
    if (inteiro->Fi > -1) {
      registradores_usados[inteiro->Fi] = 1;
    }
    inteiro++;
  }

  // Checar RAW
  Instrucao_No *instrucao = lista_leitura.cabeca;
  while (instrucao != NULL) {
    uint8_t opcode = instrucao->instrucao >> 26;
    int8_t  rd = -1, rs = -1, rt = -1;
    int16_t imm = -1, extra = -1;
    int32_t end = -1;

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

    if (banco_registradores[rs]) {
      instrucao = instrucao->proximo;
      continue;
    }

    if (opcode != 0xD && banco_registradores[rt]) {
      instrucao = instrucao->proximo;
      continue;
    }

    // Não tem RAW
    mandar_executar(instrucao);

    if (opcode < 4) {
      add_usados++;
      add = banco_uf.add;
      while (add) {
        if (!add->busy) {
          add->busy     = 1;
          add->operacao = opcode;
          add->Fi       = rd;
          add->Fj       = 0;
          add->Fk       = 0;
          add->Qj       = 0;
          add->Qk       = 0;
          add->Rj       = 1;
          add->Rk       = 1;
          break;
        }

        add++;
      }
    } else if (opcode < 6) {
      mul_usados++;
      mul = banco_uf.mul;
      while (mul) {
        if (!mul->busy) {
          mul->busy     = 1;
          mul->operacao = opcode;
          mul->Fi       = rd;
          mul->Fj       = 0;
          mul->Fk       = 0;
          mul->Qj       = 0;
          mul->Qk       = 0;
          mul->Rj       = 1;
          mul->Rk       = 1;
          break;
        }

        mul++;
      }
    } else {
      int_usados++;
      inteiro = banco_uf.inteiro;
      while (inteiro) {
        if (!inteiro->busy) {
          inteiro->busy     = 1;
          inteiro->operacao = opcode;
          inteiro->Fi       = rd;
          inteiro->Fj       = 0;
          inteiro->Fk       = 0;
          inteiro->Qj       = 0;
          inteiro->Qk       = 0;
          inteiro->Rj       = 1;
          inteiro->Rk       = 1;
          break;
        }

        inteiro++;
      }
    }

    instrucao = instrucao->proximo;
  }
}

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
}

internal void
printar_scoreboard(void) {
  local_persist uint32_t clock = 0;

  Instrucao_No *instrucao_emitida = lista_emissao.cabeca;
  Instrucao_No *instrucao_leitura = lista_leitura.cabeca;
  Instrucao_No *instrucao_executa = lista_executando.cabeca;
  Instrucao_No *instrucao_escrita = lista_escrita.cabeca;

  printf("Clock: %u\n", clock);

  // Estado das instruções
  printar_instrucoes();

  // Estado das UFs
  UF *add     = banco_uf.add;
  UF *mul     = banco_uf.mul;
  UF *inteiro = banco_uf.inteiro;

  printar_ufs();

  // Estado dos registradores
  printar_status_registradores();

  printf(
      "---------------------------------------------------------------------"
      "-----------------\n");

  clock++;
}

internal void
emitir(void) {
  Instrucao_No *instrucao_emitida    = lista_emissao.cabeca;
  Instrucao_No *instrucao_executando = lista_executando.cabeca;

  Banco_Registradores registradores_usados = { 0 };

  while (instrucao_executando != NULL) {
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

  while (instrucao_emitida != NULL) {
    uint8_t opcode = instrucao_emitida->instrucao >> 26;

    // Checa se UF disponível
    // Tá feio mas acho que tá certo
    if (opcode < 4 && add_usados < _cpu_specs.uf_add) {
    } else if (opcode < 6 && mul_usados < _cpu_specs.uf_mul) {
    } else if (opcode < 16 && int_usados < _cpu_specs.uf_int) {
    } else {
      instrucao_emitida = instrucao_emitida->proximo;
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
escrever(void) {
  Instrucao_No *instrucao = lista_escrita.cabeca;

  UF *add     = banco_uf.add;
  UF *mul     = banco_uf.mul;
  UF *inteiro = banco_uf.inteiro;

  Banco_Registradores registradores_usados = { 0 };

  for (uint i = 0; i < _cpu_specs.uf_add; i++) {
    if (add->Qj) {
      registradores_usados[add->Qj] = 1;
    }
    if (add->Qk) {
      registradores_usados[add->Qk] = 1;
    }

    add++;
  }

  for (uint i = 0; i < _cpu_specs.uf_mul; i++) {
    if (mul->Qj) {
      registradores_usados[mul->Qj] = 1;
    }
    if (mul->Qk) {
      registradores_usados[mul->Qk] = 1;
    }

    mul++;
  }

  for (uint i = 0; i < _cpu_specs.uf_int; i++) {
    if (inteiro->Qj) {
      registradores_usados[inteiro->Qj] = 1;
    }
    if (inteiro->Qk) {
      registradores_usados[inteiro->Qk] = 1;
    }

    inteiro++;
  }

  while (instrucao != NULL) {
    uint8_t opcode = instrucao->instrucao >> 26;
    if (opcode < 4) {
      add = banco_uf.add;
      for (int i = 0; i < instrucao->uf; i++) {
        add++;
      }

      if (registradores_usados[add->Fi]) {
        instrucao = instrucao->proximo;
        continue;
      }

      add->busy = 0;
      add_usados--;

      barramento_escrever_dado(0, banco_registradores[add->Fi]);
    } else if (opcode < 6) {
      mul = banco_uf.mul;
      for (int i = 0; i < instrucao->uf; i++) {
        mul++;
      }

      if (registradores_usados[mul->Fi]) {
        instrucao = instrucao->proximo;
        continue;
      }

      mul->busy = 0;
      mul_usados--;

      barramento_escrever_dado(0, banco_registradores[mul->Fi]);
    } else if (opcode < 16) {
      inteiro = banco_uf.inteiro;
      for (int i = 0; i < instrucao->uf; i++) {
        inteiro++;
      }

      if (registradores_usados[inteiro->Fi]) {
        instrucao = instrucao->proximo;
        continue;
      }

      inteiro->busy = 0;
      int_usados--;

      barramento_escrever_dado(0, banco_registradores[inteiro->Fi]);
    } else {
      rodando = 0;
    }

    instrucao = instrucao->proximo;
  }
}

internal void
executar(void) {
  Instrucao_No *instrucao = lista_executando.cabeca;

  while (instrucao != NULL) {
    if (instrucao->clocks_restantes > 0) {
      instrucao->clocks_restantes--;
    } else {
      // TODO: Atualizar UF

      // Mandar para escrita
      mandar_escrever(instrucao);
    }

    instrucao = instrucao->proximo;
  }
}

internal void
mandar_ler(Instrucao_No *instrucao) {
  Instrucao_No *emitida = lista_emissao.cabeca;

  if (emitida == instrucao) {
    lista_emissao.cabeca = NULL;
    lista_emissao.fim    = NULL;

    if (lista_leitura.cabeca == NULL) {
      lista_leitura.cabeca = instrucao;
      lista_leitura.fim    = instrucao;
    } else {
      lista_leitura.fim->proximo = instrucao;
      lista_leitura.fim          = instrucao;
    }
  } else {
    while (emitida && emitida->proximo != instrucao) {
      emitida = emitida->proximo;
    }

    if (emitida) {
      emitida->proximo = instrucao->proximo;

      if (lista_leitura.cabeca == NULL) {
        lista_leitura.cabeca = instrucao;
        lista_leitura.fim    = instrucao;
      } else {
        lista_leitura.fim->proximo = instrucao;
        lista_leitura.fim          = instrucao;
      }
    }
  }
}

internal void
mandar_executar(Instrucao_No *instrucao) {
  Instrucao_No *leitura = lista_leitura.cabeca;

  if (leitura == instrucao) {
    lista_leitura.cabeca = NULL;
    lista_leitura.fim    = NULL;

    if (lista_executando.cabeca == NULL) {
      lista_executando.cabeca = instrucao;
      lista_executando.fim    = instrucao;
    } else {
      lista_executando.fim->proximo = instrucao;
      lista_executando.fim          = instrucao;
    }
  } else {
    while (leitura && leitura->proximo != instrucao) {
      leitura = leitura->proximo;
    }

    if (leitura) {
      leitura->proximo = instrucao->proximo;

      if (lista_executando.cabeca == NULL) {
        lista_executando.cabeca = instrucao;
        lista_executando.fim    = instrucao;
      } else {
        lista_executando.fim->proximo = instrucao;
        lista_executando.fim          = instrucao;
      }
    }
  }
}

internal void
mandar_escrever(Instrucao_No *instrucao) {
  Instrucao_No *executando = lista_executando.cabeca;

  if (executando == instrucao) {
    lista_executando.cabeca = NULL;
    lista_executando.fim    = NULL;

    if (lista_escrita.cabeca == NULL) {
      lista_escrita.cabeca = instrucao;
      lista_escrita.fim    = instrucao;
    } else {
      lista_escrita.fim->proximo = instrucao;
      lista_escrita.fim          = instrucao;
    }
  } else {
    while (executando && executando->proximo != instrucao) {
      executando = executando->proximo;
    }

    if (executando) {
      executando->proximo = instrucao->proximo;

      if (lista_escrita.cabeca == NULL) {
        lista_escrita.cabeca = instrucao;
        lista_escrita.fim    = instrucao;
      } else {
        lista_escrita.fim->proximo = instrucao;
        lista_escrita.fim          = instrucao;
      }
    }
  }
}

internal void
printar_ufs(void) {
  UF *add     = banco_uf.add;
  UF *mul     = banco_uf.mul;
  UF *inteiro = banco_uf.inteiro;

  printf("\nName\tBusy\tOperation\tFi\tFj\tFk\tQj\tQk\tRj\tRk\n");

  printf(
      "------------------------------------------------------------------------"
      "-----------------\n");

  for (uint i = 0; i < _cpu_specs.uf_add; i++) {
    printf("ADD %u:\t%u\t%u\t\t%u\t%u\t%d\t%d\t%d\t%d\t%d|\n", i, add->busy,
           add->operacao, add->Fi, add->Fj, add->Fk, add->Qj, add->Qk, add->Rj,
           add->Rk);
    add++;
  }

  printf(
      "------------------------------------------------------------------------"
      "-----------------\n");

  for (uint i = 0; i < _cpu_specs.uf_mul; i++) {
    printf("MUL %u:\t%u\t%u\t\t%u\t%u\t%d\t%d\t%d\t%d\t%d|\n", i, mul->busy,
           mul->operacao, mul->Fi, mul->Fj, mul->Fk, mul->Qj, mul->Qk, mul->Rj,
           mul->Rk);
    mul++;
  }

  printf(
      "------------------------------------------------------------------------"
      "-----------------\n");

  for (uint i = 0; i < _cpu_specs.uf_int; i++) {
    printf("INT %u:\t%u\t%u\t\t%u\t%u\t%d\t%d\t%d\t%d\t%d|\n", i, inteiro->busy,
           inteiro->operacao, inteiro->Fi, inteiro->Fj, inteiro->Fk,
           inteiro->Qj, inteiro->Qk, inteiro->Rj, inteiro->Rk);
    inteiro++;
  }

  printf(
      "------------------------------------------------------------------------"
      "-----------------\n");
}

internal void
printar_status_registradores(void) {
  printf("\nRegistradores:\n\n");

  for (uint i = 0; i < 32; i++) {
    char *uf = Tipo_UF_Nome[status_registrador[i].uf];
    printf("R%u:\t%s\t%u\n", i, uf, status_registrador[i].pos);
  }
}

internal void
printar_instrucoes(void) {
  printf("\nInstruções:\n\n");
  Status_Instrucoes *instrucao = status_instrucoes;

  for (uint i = 0; i < _cpu_specs.qtd_instrucoes; i++) {
    printf("%s\t%s\t%s\t%s\t%s\t%s\n", instrucao->instrucao,
           instrucao->busca ? "X" : "-", instrucao->emissao ? "X" : "-",
           instrucao->leitura ? "X" : "-", instrucao->execucao ? "X" : "-",
           instrucao->escrita ? "X" : "-");
    instrucao++;
  }
}
