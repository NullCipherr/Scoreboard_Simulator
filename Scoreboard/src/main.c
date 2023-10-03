#include "barramento.h"
#include "cpu.h"
#include "defs.h"
#include "hashtables.h"
#include "leitor.h"
#include "memoria.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  uint32_t tamanho_memoria;
  char    *nome_programa;
  char    *nome_saida;
} Programa;

internal void definir_programa(Programa *programa, int argc, char *argv[]);
internal void print_ajuda(void);

int
main(int argc, char *argv[]) {
  Programa programa = { 0 };
  definir_programa(&programa, argc, argv);

  char     *memoria   = (char *) malloc(programa.tamanho_memoria);
  CPU_Specs cpu_specs = { 0 };

  FILE *arq = fopen(programa.nome_programa, "r");
  if (arq == NULL) {
    fprintf(stderr, "Erro ao abrir o arquivo.\n");
    exit(1);
  }

  fseek(arq, 0, SEEK_END);
  unsigned long tamanho = ftell(arq);
  rewind(arq);

  char buffer[tamanho];
  fread(buffer, sizeof(char), tamanho, arq);
  buffer[tamanho] = '\0';
  fclose(arq);

  printf("%s\n", buffer);

  leitor_ler_arquivo(buffer, memoria, &cpu_specs);
  printf("%d\n", *((int *) &memoria[00]));

  scoreboard_inicializar(&cpu_specs);
  barramento_inicializar(memoria);
  rodar_programa(programa.nome_saida, &cpu_specs);

  return 0;
}

internal void
definir_programa(Programa *programa, int argc, char *argv[]) {
  int opt;
  while ((opt = getopt(argc, argv, "p:m:o:h")) != -1) {
    switch (opt) {
    case 'p':
      if (strncmp(optarg + strlen(optarg) - 4, ".asb", 4) == 0) {
        programa->nome_programa = optarg;
      } else {
        fprintf(stderr,
                "Extensão inválida, por favor use um arquivo \".asb\".\n");
        exit(1);
      }
      break;

    case 'm':
      programa->tamanho_memoria = atoi(optarg);
      if (programa->tamanho_memoria < 256) {
        fprintf(stderr, "Tamanho de memória muito pequeno. Minímo 256.\n");
        exit(1);
      }
      break;

    case 'o': programa->nome_saida = optarg; break;

    case 'h': print_ajuda(); exit(0);
    }
  }

  if (programa->nome_saida == NULL) {
    programa->nome_saida = "scoreboard.out";
  }
}

internal void
print_ajuda(void) {
  printf(
      "Uso: ./scoreboarding -p <arquivo> -m <tamanho> [-o <nome>]\n"
      "  -p <arquivo>   Arquivo com o código em assembly.\n"
      "  -m <tamanho>   Tamanho da memória em bytes.\n"
      "  -o <nome>      Nome do arquivo de saída. Padrão: "
      "\"scoreboard.out\"\n");
}
