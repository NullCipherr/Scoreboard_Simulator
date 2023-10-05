#pragma once

#include "cpu.h"

#include <stddef.h>
#include <stdint.h>

typedef struct _leitor Leitor;

struct _leitor {
  char  *input;
  char   ch;
  size_t pos;
  size_t prox_pos;
};

void leitor_ler_arquivo(char *input, char *memoria, CPU_Specs *cpu_specs)
    __attribute__((nonnull));
