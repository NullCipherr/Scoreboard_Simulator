#include "leitor.h"

#include "cpu.h"
#include "defs.h"
#include "hashtables.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

internal void  prosseguir(Leitor *l);
internal char  espiar(Leitor *l);
internal void  ignorar_espacos(Leitor *l);
internal void  ler_ufs(Leitor *l, CPU_Specs *cpu_specs);
internal void  ler_specs(Leitor *l, CPU_Specs *cpu_specs);
internal char *ler_identificador(Leitor *l);
internal void  ignorar_comentario(Leitor *l);
internal void  ler_dados(Leitor *l, char *memoria, uint32_t *idx);
internal void  ler_texto(Leitor *l, char *memoria, uint32_t *idx);
internal void  ler_campos_r(Leitor *l, char *memoria, uint32_t *idx,
                            const uint8_t opcode);
internal void  ler_campos_i_1(Leitor *l, char *memoria, uint32_t *idx);
internal void  ler_campos_i_2(Leitor *l, char *memoria, uint32_t *idx);
internal void  ler_campos_i_3(Leitor *l, char *memoria, uint32_t *idx);
internal void  ler_campos_j(Leitor *l, char *memoria, uint32_t *idx);
internal void  ler_clocks(Leitor *l, CPU_Specs *cpu_specs);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                          PUBLIC FUNCTIONS                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void
leitor_ler_arquivo(char *input, char *memoria, CPU_Specs *cpu_specs) {
  Leitor l;

  l.input    = input;
  l.ch       = '\0';
  l.pos      = 0;
  l.prox_pos = 0;

  prosseguir(&l);

  uint32_t idx = 0;

  // Faz a leitura do arquivo e salva as informações na memória.
  while (l.ch != '\0') {
    ignorar_espacos(&l);

    switch (l.ch) {
    case '/':
      if (espiar(&l) == '*') {
        prosseguir(&l);
        prosseguir(&l);
        ler_specs(&l, cpu_specs);
      } else {
        perror("Erro de sintaxe 1");
        exit(EXIT_FAILURE);
      }
      break;

    case '.':
      if (isalpha(espiar(&l))) {
        prosseguir(&l);
        char *identificador = ler_identificador(&l);
        if (strncmp(identificador, "data", 4) == 0) {
          ler_dados(&l, memoria, &idx);
        } else if (strncmp(identificador, "text", 4) == 0) {
          ler_texto(&l, memoria, &idx);
        } else {
          perror("Erro de sintaxe 2");
          exit(EXIT_FAILURE);
        }
      } else {
        perror("Erro de sintaxe 3");
        exit(EXIT_FAILURE);
      }
      break;

    case '#': ignorar_comentario(&l); break;
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                          PRIVATE FUNCTIONS                              *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal void
prosseguir(Leitor *l) {
  if (l->prox_pos < strlen(l->input)) {
    l->pos = l->prox_pos;
    l->ch  = l->input[l->prox_pos++];
  } else {
    l->ch = '\0';
  }
}

internal void
ignorar_espacos(Leitor *l) {
  while (isspace(l->ch) != 0) {
    prosseguir(l);
  }
}

internal void
ignorar_comentario(Leitor *l) {
  while (l->ch != '\n') {
    prosseguir(l);
  }
}

internal char
espiar(Leitor *l) {
  if (l->prox_pos < strlen(l->input)) {
    return l->input[l->prox_pos];
  } else {
    return '\0';
  }
}

internal char *
ler_identificador(Leitor *l) {
  char *palavra;
  int   pos     = l->pos;
  int   tamanho = 0;

  while (isalnum(l->ch) != 0) {
    prosseguir(l);
    tamanho++;
  }

  palavra = malloc(pos + 1);
  strncpy(palavra, l->input + pos, tamanho);

  return palavra;
}

internal void
ler_dados(Leitor *l, char *memoria, uint32_t *idx) {
  while (l->ch != '\0' && l->ch != '.') {
    ignorar_espacos(l);
    if (isdigit(l->ch)) {
      char *identificador        = ler_identificador(l);
      *((int *) &memoria[*idx])  = atoi(identificador);
      (*idx)                    += 4;
    }
  }
}

internal void
ler_texto(Leitor *l, char *memoria, uint32_t *idx) {
  struct OpCodeMap op_code_map = { 0 };

  *idx = 100; // Primeiros 100 bits são reservados para os dados.

  while (l->ch != '\0') {
    ignorar_espacos(l);
    if (isalnum(l->ch)) {
      char *palavra              = ler_identificador(l);
      op_code_map                = encontra_operacao(palavra, strlen(palavra));
      *((int *) &memoria[*idx]) |= op_code_map.opcode << 26;

      switch (op_code_map.t) {
      case R: // add, sub, mul, div, and, or, not
        ler_campos_r(l, memoria, idx, op_code_map.opcode);
        break;
      case I:
        switch (op_code_map.opcode) {
        case 0x1: // addi
        case 0x3: // subi
          ler_campos_i_1(l, memoria, idx);
          break;
        case 0x9: // blt
        case 0xA: // bgt
        case 0xB: // beq
        case 0xC: // bne
          ler_campos_i_2(l, memoria, idx);
          break;
        case 0xE: // lw
        case 0xF: // sw
          ler_campos_i_3(l, memoria, idx);
          break;
        }
        break;
      case J: // j, exit
        if (op_code_map.opcode == 0xD) {
          ler_campos_j(l, memoria, idx);
        } else {
          *((int *) &memoria[*idx]) |= 0x0;
        }
        break;
      }
      (*idx) += 4;
    } else if (l->ch == '#') {
      ignorar_comentario(l);
    }
  }
}

internal void
ler_specs(Leitor *l, CPU_Specs *cpu_specs) {
  while (l->ch != '\0' && l->ch != '*') {
    ignorar_espacos(l);

    if (isalpha(l->ch)) {
      char *identificador = ler_identificador(l);
      if (strncmp(identificador, "UF", 2) == 0) {
        prosseguir(l);
        ler_ufs(l, cpu_specs);
      } else if (strncmp(identificador, "INST", 4) == 0) {
        prosseguir(l);
        ler_clocks(l, cpu_specs);
      } else {
        perror("Erro de sintaxe 4");
        exit(EXIT_FAILURE);
      }
    } else if (l->ch == '.') {
      break;
    }
    // else {
    //   perror("Erro de sintaxe 5");
    //   exit(EXIT_FAILURE);
    // }
  }

  // Talvez pode dar problema.
  // if (l->ch == '*' && espiar(l) == '/') {
  //   prosseguir(l);
  //   prosseguir(l);
  // } else {
  //   perror("Erro de sintaxe 6");
  //   exit(EXIT_FAILURE);
  // }
}

internal void
ler_campos_r(Leitor *l, char *memoria, uint32_t *idx, const uint8_t opcode) {
  // Ignora o espaço entre a operação e o primeiro registrador.
  ignorar_espacos(l);

  if (l->ch == 'r') {
    char   *identificador = ler_identificador(l);
    uint8_t registrador   = encontra_reg(identificador, strlen(identificador));
    *((int *) &memoria[*idx]) |= registrador << 11;
  } else {
    perror("Erro de sintaxe 7");
    exit(EXIT_FAILURE);
  }

  if (l->ch == ',') {
    prosseguir(l);
  } else {
    perror("Erro de sintaxe 8");
    exit(EXIT_FAILURE);
  }

  // Ignora o espaço entre o primeiro e o segundo registrador.
  ignorar_espacos(l);

  if (l->ch == 'r') {
    char   *identificador = ler_identificador(l);
    uint8_t registrador   = encontra_reg(identificador, strlen(identificador));
    *((int *) &memoria[*idx]) |= registrador << 21;
  } else {
    perror("Erro de sintaxe 9");
    exit(EXIT_FAILURE);
  }

  if (opcode == 0x8) {
    return;
  }

  if (l->ch == ',') {
    prosseguir(l);
  } else {
    perror("Erro de sintaxe 10");
    exit(EXIT_FAILURE);
  }

  // Ignora o espaço entre o segundo e o terceiro registrador.
  ignorar_espacos(l);

  if (l->ch == 'r') {
    char   *identificador = ler_identificador(l);
    uint8_t registrador   = encontra_reg(identificador, strlen(identificador));
    *((int *) &memoria[*idx]) |= registrador << 16;
  } else {
    perror("Erro de sintaxe 11");
    exit(EXIT_FAILURE);
  }
}

internal void
ler_campos_i_1(Leitor *l, char *memoria, uint32_t *idx) {
  // Ignora o espaço entre a operação e o primeiro registrador.
  ignorar_espacos(l);

  if (l->ch == 'r') {
    char   *identificador = ler_identificador(l);
    uint8_t registrador   = encontra_reg(identificador, strlen(identificador));
    *((int *) &memoria[*idx]) |= registrador << 16;
  } else {
    perror("Erro de sintaxe 12");
    exit(EXIT_FAILURE);
  }

  if (l->ch == ',') {
    prosseguir(l);
  } else {
    perror("Erro de sintaxe 13");
    exit(EXIT_FAILURE);
  }

  // Ignora o espaço entre o primeiro e o segundo registrador.
  ignorar_espacos(l);

  if (l->ch == 'r') {
    char   *identificador = ler_identificador(l);
    uint8_t registrador   = encontra_reg(identificador, strlen(identificador));
    *((int *) &memoria[*idx]) |= registrador << 21;
  } else {
    perror("Erro de sintaxe 14");
    exit(EXIT_FAILURE);
  }

  if (l->ch == ',') {
    prosseguir(l);
  } else {
    perror("Erro de sintaxe 15");
    exit(EXIT_FAILURE);
  }

  // Ignora o espaço entre o segundo registrador e o valor imediato.
  ignorar_espacos(l);

  if (isdigit(l->ch)) {
    char *palavra              = ler_identificador(l);
    *((int *) &memoria[*idx]) |= atoi(palavra);
  } else {
    perror("Erro de sintaxe 16");
    exit(EXIT_FAILURE);
  }
}

internal void
ler_campos_i_2(Leitor *l, char *memoria, uint32_t *idx) {
  // Ignora o espaço entre a operação e o primeiro registrador.
  ignorar_espacos(l);

  if (l->ch == 'r') {
    char   *identificador = ler_identificador(l);
    uint8_t registrador   = encontra_reg(identificador, strlen(identificador));
    *((int *) &memoria[*idx]) |= registrador << 21;
  } else {
    perror("Erro de sintaxe 17");
    exit(EXIT_FAILURE);
  }

  if (l->ch == ',') {
    prosseguir(l);
  } else {
    perror("Erro de sintaxe 18");
    exit(EXIT_FAILURE);
  }

  // Ignora o espaço entre o primeiro e o segundo registrador.
  ignorar_espacos(l);

  if (l->ch == 'r') {
    char   *identificador = ler_identificador(l);
    uint8_t registrador   = encontra_reg(identificador, strlen(identificador));
    *((int *) &memoria[*idx]) |= registrador << 16;
  } else {
    perror("Erro de sintaxe 19");
    exit(EXIT_FAILURE);
  }

  if (l->ch == ',') {
    prosseguir(l);
  } else {
    perror("Erro de sintaxe 20");
    exit(EXIT_FAILURE);
  }

  // Ignora o espaço entre o segundo registrador e o valor imediato.
  ignorar_espacos(l);

  if (isdigit(l->ch)) {
    char *palavra              = ler_identificador(l);
    *((int *) &memoria[*idx]) |= atoi(palavra);
  } else {
    perror("Erro de sintaxe 21");
    exit(EXIT_FAILURE);
  }
}

internal void
ler_campos_i_3(Leitor *l, char *memoria, uint32_t *idx) {
  // Ignorar o espaço entre a operação e o primeiro registrador.
  ignorar_espacos(l);

  if (l->ch == 'r') {
    char   *identificador = ler_identificador(l);
    uint8_t registrador   = encontra_reg(identificador, strlen(identificador));
    *((int *) &memoria[*idx]) |= registrador << 16;
  } else {
    perror("Erro de sintaxe 22");
    exit(EXIT_FAILURE);
  }

  if (l->ch == ',') {
    prosseguir(l);
  } else {
    perror("Erro de sintaxe 23");
    exit(EXIT_FAILURE);
  }

  // Ignorar o espaço entre o primeiro registrador e o valor imediato.
  ignorar_espacos(l);

  if (isdigit(l->ch)) {
    char *palavra              = ler_identificador(l);
    *((int *) &memoria[*idx]) |= atoi(palavra);
  } else {
    perror("Erro de sintaxe 24");
    exit(EXIT_FAILURE);
  }

  if (l->ch == '(') {
    prosseguir(l);
  } else {
    perror("Erro de sintaxe 25");
    exit(EXIT_FAILURE);
  }

  // Ignorar o espaço entre o valor imediato e o segundo registrador.
  ignorar_espacos(l);

  if (l->ch == 'r') {
    char   *identificador = ler_identificador(l);
    uint8_t registrador   = encontra_reg(identificador, strlen(identificador));
    *((int *) &memoria[*idx]) |= registrador << 21;
  } else {
    perror("Erro de sintaxe 26");
    exit(EXIT_FAILURE);
  }

  if (l->ch == ')') {
    prosseguir(l);
  } else {
    perror("Erro de sintaxe 27");
    exit(EXIT_FAILURE);
  }
}

internal void
ler_campos_j(Leitor *l, char *memoria, uint32_t *idx) {
  // Ignorar o espaço entre a operação e o valor imediato.
  ignorar_espacos(l);

  if (isdigit(l->ch)) {
    char *palavra              = ler_identificador(l);
    *((int *) &memoria[*idx]) |= atoi(palavra);
  } else {
    perror("Erro de sintaxe 28");
    exit(EXIT_FAILURE);
  }
}

internal void
ler_clocks(Leitor *l, CPU_Specs *cpu_specs) {
  while (l->ch != '\0' || l->ch != '*') {
    ignorar_espacos(l);

    if (isalpha(l->ch)) {
      char *identificador = ler_identificador(l);
      if (strncmp(identificador, "add", 3) == 0) {
        if (l->ch == ':') {
          prosseguir(l);
        } else {
          perror("Erro de sintaxe 29");
          exit(EXIT_FAILURE);
        }
        ignorar_espacos(l);
        cpu_specs->clock_add = atoi(ler_identificador(l));
      } else if (strncmp(identificador, "addi", 4) == 0) {
        if (l->ch == ':') {
          prosseguir(l);
        } else {
          perror("Erro de sintaxe 30");
          exit(EXIT_FAILURE);
        }
        ignorar_espacos(l);
        cpu_specs->clock_addi = atoi(ler_identificador(l));
      } else if (strncmp(identificador, "sub", 3) == 0) {
        if (l->ch == ':') {
          prosseguir(l);
        } else {
          perror("Erro de sintaxe 31");
          exit(EXIT_FAILURE);
        }
        ignorar_espacos(l);
        cpu_specs->clock_sub = atoi(ler_identificador(l));
      } else if (strncmp(identificador, "subi", 4) == 0) {
        if (l->ch == ':') {
          prosseguir(l);
        } else {
          perror("Erro de sintaxe 32");
          exit(EXIT_FAILURE);
        }
        ignorar_espacos(l);
        cpu_specs->clock_subi = atoi(ler_identificador(l));
      } else if (strncmp(identificador, "mul", 3) == 0) {
        if (l->ch == ':') {
          prosseguir(l);
        } else {
          perror("Erro de sintaxe 33");
          exit(EXIT_FAILURE);
        }
        ignorar_espacos(l);
        cpu_specs->clock_mul = atoi(ler_identificador(l));
      } else if (strncmp(identificador, "div", 3) == 0) {
        if (l->ch == ':') {
          prosseguir(l);
        } else {
          perror("Erro de sintaxe 34");
          exit(EXIT_FAILURE);
        }
        ignorar_espacos(l);
        cpu_specs->clock_mul = atoi(ler_identificador(l));
      } else if (strncmp(identificador, "and", 3) == 0) {
        if (l->ch == ':') {
          prosseguir(l);
        } else {
          perror("Erro de sintaxe 35");
          exit(EXIT_FAILURE);
        }
        ignorar_espacos(l);
        cpu_specs->clock_and = atoi(ler_identificador(l));
      } else if (strncmp(identificador, "or", 2) == 0) {
        if (l->ch == ':') {
          prosseguir(l);
        } else {
          perror("Erro de sintaxe 36");
          exit(EXIT_FAILURE);
        }
        ignorar_espacos(l);
        cpu_specs->clock_or = atoi(ler_identificador(l));
      } else if (strncmp(identificador, "not", 3) == 0) {
        if (l->ch == ':') {
          prosseguir(l);
        } else {
          perror("Erro de sintaxe 37");
          exit(EXIT_FAILURE);
        }
        ignorar_espacos(l);
        cpu_specs->clock_not = atoi(ler_identificador(l));
      } else if (strncmp(identificador, "blt", 3) == 0) {
        if (l->ch == ':') {
          prosseguir(l);
        } else {
          perror("Erro de sintaxe 38");
          exit(EXIT_FAILURE);
        }
        ignorar_espacos(l);
        cpu_specs->clock_blt = atoi(ler_identificador(l));
      } else if (strncmp(identificador, "bgt", 3) == 0) {
        if (l->ch == ':') {
          prosseguir(l);
        } else {
          perror("Erro de sintaxe 39");
          exit(EXIT_FAILURE);
        }
        ignorar_espacos(l);
        cpu_specs->clock_bgt = atoi(ler_identificador(l));
      } else if (strncmp(identificador, "beq", 3) == 0) {
        if (l->ch == ':') {
          prosseguir(l);
        } else {
          perror("Erro de sintaxe 40");
          exit(EXIT_FAILURE);
        }
        ignorar_espacos(l);
        cpu_specs->clock_beq = atoi(ler_identificador(l));
      } else if (strncmp(identificador, "bne", 3) == 0) {
        if (l->ch == ':') {
          prosseguir(l);
        } else {
          perror("Erro de sintaxe 41");
          exit(EXIT_FAILURE);
        }
        ignorar_espacos(l);
        cpu_specs->clock_bne = atoi(ler_identificador(l));
      } else if (strncmp(identificador, "j", 1) == 0) {
        if (l->ch == ':') {
          prosseguir(l);
        } else {
          perror("Erro de sintaxe");
          exit(EXIT_FAILURE);
        }
        ignorar_espacos(l);
        cpu_specs->clock_jump = atoi(ler_identificador(l));
      } else if (strncmp(identificador, "lw", 2) == 0) {
        if (l->ch == ':') {
          prosseguir(l);
        } else {
          perror("Erro de sintaxe 42");
          exit(EXIT_FAILURE);
        }
        ignorar_espacos(l);
        cpu_specs->clock_load = atoi(ler_identificador(l));
      } else if (strncmp(identificador, "sw", 2) == 0) {
        if (l->ch == ':') {
          prosseguir(l);
        } else {
          perror("Erro de sintaxe 43");
          exit(EXIT_FAILURE);
        }
        ignorar_espacos(l);
        cpu_specs->clock_store = atoi(ler_identificador(l));
      } else if (strncmp(identificador, "UF", 2) == 0) {
        prosseguir(l);
        ler_ufs(l, cpu_specs);
        break;
      }
    } else if (l->ch == '*' && espiar(l) == '/') {
      prosseguir(l);
      prosseguir(l);
      break;
    } else {
      perror("Erro de sintaxe 44");
      exit(EXIT_FAILURE);
    }
  }

  // Talvez pode dar problema
  // if (l->ch == '*' && espiar(l) == '/') {
  //   prosseguir(l);
  //   prosseguir(l);
  // } else {
  //   perror("Erro de sintaxe 45");
  //   exit(EXIT_FAILURE);
  // }
}

internal void
ler_ufs(Leitor *l, CPU_Specs *cpu_specs) {
  while (l->ch != '\0' && l->ch != '*') {
    ignorar_espacos(l);

    if (isalpha(l->ch)) {
      char *identificador = ler_identificador(l);
      if (strncmp(identificador, "add", 3) == 0) {
        if (l->ch == ':') {
          prosseguir(l);
        } else {
          perror("Erro de sintaxe 46");
          exit(EXIT_FAILURE);
        }
        ignorar_espacos(l);
        identificador     = ler_identificador(l);
        cpu_specs->uf_add = atoi(identificador);
      } else if (strncmp(identificador, "mul", 3) == 0) {
        if (l->ch == ':') {
          prosseguir(l);
        } else {
          perror("Erro de sintaxe 47");
          exit(EXIT_FAILURE);
        }
        ignorar_espacos(l);
        cpu_specs->uf_mul = atoi(ler_identificador(l));
      } else if (strncmp(identificador, "inteiro", 7) == 0) {
        if (l->ch == ':') {
          prosseguir(l);
        } else {
          perror("Erro de sintaxe 48");
          exit(EXIT_FAILURE);
        }
        ignorar_espacos(l);
        cpu_specs->uf_int = atoi(ler_identificador(l));
      } else if (strncmp(identificador, "INST", 4) == 0) {
        prosseguir(l);
        ler_clocks(l, cpu_specs);
        break;
      }
    } else {
      perror("Erro de sintaxe 49");
      exit(EXIT_FAILURE);
    }
  }

  // Talvez pode dar problema.
  // if (l->ch == '*' && espiar(l) == '/') {
  //   prosseguir(l);
  //   prosseguir(l);
  // } else {
  //   perror("Erro de sintaxe 50");
  //   exit(EXIT_FAILURE);
  // }
}
