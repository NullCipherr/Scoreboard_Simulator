#include <stdio.h>

// Registradores do processador
int registradores[32];

// Estrutura para representar a memória
struct Memoria {
  int dados[1024]; // Memória fictícia com 1024 palavras
};

// Declaração de uma variável de memória
struct Memoria memoria;

void imprimir_memoria() {
  printf("Conteúdo da memória:\n");
  for (int i = 0; i < 1024; i++) {
    printf("memoria[%d] = %d\n", i, memoria.dados[i]);
  }
}

// Função para ler dados da memória
int ler_memoria(int endereco) {
  if (endereco >= 0 && endereco < 1024) {
    return memoria.dados[endereco]; // Corrigido para usar memoria.dados
  } else {
    // Tratar erro de acesso à memória fora dos limites
    printf("Erro: Acesso à memória fora dos limites.\n");

    return 0;
  }
}

// Função para escrever dados na memória
void escrever_memoria(int endereco, int dado) {
  if (endereco >= 0 && endereco < 1024) {
    memoria.dados[endereco] = dado; // Corrigido para usar memoria.dados
  } else {
    // Tratar erro de acesso à memória fora dos limites
    printf("Erro: Acesso à memória fora dos limites.\n");
  }
}

// Rótulos para os registradores
const char *nomes_registradores[32] = {
    "r0",  "r1",  "r2",  "r3",  "r4",  "r5",  "r6",  "r7",  "r8",  "r9",  "r10",
    "r11", "r12", "r13", "r14", "r15", "r16", "r17", "r18", "r19", "r20", "r21",
    "r22", "r23", "r24", "r25", "r26", "r27", "r28", "r29", "r30", "r31"};

// Operações do processador
enum OpCode {
  ADD = 0x00,
  ADDI = 0x01,
  SUB = 0x02,
  SUBI = 0x03,
  MUL = 0x04,
  DIV = 0x05,
  AND = 0x06,
  OR = 0x07,
  NOT = 0x08,
  BLT = 0x09,
  BGT = 0x0A,
  BEQ = 0x0B,
  BNE = 0x0C,
  JUMP = 0x0D,
  LW = 0x0E,
  SW = 0x0F,
  EXIT = 0x10,
  // Adicione outras operações aqui conforme necessário
};

// Tipos de instrução
enum InstrucaoTipo { R, I, J };

// Endereço do próximo comando a ser executado
int pc = 0;

// Declaração de barramentos
int barramento_leitura =
    -1; // Inicialmente, nenhum registrador está usando o barramento de leitura
int barramento_escrita =
    -1; // Inicialmente, nenhum registrador está usando o barramento de escrita

// Função para ler do barramento de leitura
int ler_barramento() {
  if (barramento_leitura != -1) {
    printf("Barramento de leitura conectado a %s\n",
           nomes_registradores[barramento_leitura]);

    return registradores[barramento_leitura];
  }
  return 0; // Se não estiver conectado, retorna 0
}

// Função para escrever no barramento de escrita
void escrever_barramento(int valor) {
  if (barramento_escrita != -1) {
    printf("Barramento de escrita conectado a %s\n",
           nomes_registradores[barramento_escrita]);
    registradores[barramento_escrita] = valor;
  }
}

// Executa uma instrução
void executar_instrucao(enum OpCode opcode, int rd, int rs, int rt, int imm) {
  int resultado;
  const char *nome_rd = nomes_registradores[rd];
  const char *nome_rs = nomes_registradores[rs];
  const char *nome_rt = nomes_registradores[rt];

  switch (opcode) {
  case ADD:
    registradores[rd] = registradores[rs] + registradores[rt];
    printf("ADD %s, %s, %s\n", nome_rd, nome_rs, nome_rt);
    break;
  case ADDI:
    registradores[rd] = registradores[rs] + imm;
    printf("ADDI %s, %s, %d\n", nome_rd, nome_rs, imm);
    break;
  case SUB:
    registradores[rd] = registradores[rs] - registradores[rt];
    printf("SUB %s, %s, %s\n", nome_rd, nome_rs, nome_rt);
    break;
  case SUBI:
    registradores[rd] = registradores[rs] - imm;
    printf("SUBI %s, %s, %d\n", nome_rd, nome_rs, imm);
    break;
  case MUL:
    registradores[rd] = registradores[rs] * registradores[rt];
    printf("MUL %s, %s, %s\n", nome_rd, nome_rs, nome_rt);
    break;
  case DIV:
    registradores[rd] = registradores[rs] / registradores[rt];
    printf("DIV %s, %s, %s\n", nome_rd, nome_rs, nome_rt);
    break;
  case AND:
    registradores[rd] = registradores[rs] & registradores[rt];
    printf("AND %s, %s, %s\n", nome_rd, nome_rs, nome_rt);
    break;
  case OR:
    registradores[rd] = registradores[rs] | registradores[rt];
    printf("OR %s, %s, %s\n", nome_rd, nome_rs, nome_rt);
    break;
  case NOT:
    registradores[rd] = ~registradores[rs];
    printf("NOT %s, %s\n", nome_rd, nome_rs);
    break;
  case BLT:
    if (registradores[rs] < registradores[rt]) {
      pc = imm; // Salta para o endereço imediato
      printf("BLT %s, %s, %d (Salto para %d)\n", nome_rs, nome_rt, imm, pc);
    }
    break;
  case BGT:
    if (registradores[rs] > registradores[rt]) {
      pc = imm; // Salta para o endereço imediato
      printf("BGT %s, %s, %d (Salto para %d)\n", nome_rs, nome_rt, imm, pc);
    }
    break;
  case BEQ:
    if (registradores[rs] == registradores[rt]) {
      pc = imm; // Salta para o endereço imediato
      printf("BEQ %s, %s, %d (Salto para %d)\n", nome_rs, nome_rt, imm, pc);
    }
    break;
  case BNE:
    if (registradores[rs] != registradores[rt]) {
      pc = imm; // Salta para o endereço imediato
      printf("BNE %s, %s, %d (Salto para %d)\n", nome_rs, nome_rt, imm, pc);
    }
    break;
  case JUMP:
    pc = imm; // Salto incondicional para o endereço imediato
    printf("JUMP %d\n", imm);
    break;
  case LW:
    // Implementação da instrução de carregamento de memória
    if (rs >= 0 && rs < 32) {
      barramento_leitura =
          rs; // Conecta o barramento de leitura ao registrador rs
      int endereco_memoria = registradores[rs] + imm;
      resultado = memoria.dados[endereco_memoria];
      barramento_leitura = -1; // Desconecta o barramento de leitura
      escrever_barramento(
          resultado); // Escreve o valor lido no barramento de escrita
      printf("LW %s, %s, %d (Valor lido: %d)\n", nome_rd, nome_rs, imm,
             resultado);
    }
    break;
  case SW:
    // Implementação da instrução de armazenamento na memória
    if (rs >= 0 && rs < 32) {
      barramento_leitura =
          rd; // Conecta o barramento de leitura ao registrador rd
      int endereco_memoria = registradores[rs] + imm;
      int valor = ler_barramento(); // Use ler_barramento para ler o valor do
                                    // barramento de leitura
      escrever_memoria(endereco_memoria,
                       valor); // Use escrever_memoria para escrever na memória
      barramento_leitura = -1; // Desconecta o barramento de leitura
      printf("SW %s, %s, %d (Valor escrito: %d)\n", nome_rd, nome_rs, imm,
             valor);
    }
    break;
  case EXIT:
    // Implementação da instrução de saída
    printf("Programa encerrado.\n");
    pc = -1; // Termina o programa
    break;
  // Adicione outras operações aqui conforme necessário
  default:
    printf("Opcode desconhecido\n");
    break;
  }
}

int main() {
  // Inicializa registradores
  for (int i = 0; i < 32; i++) {
    registradores[i] = 0;
  }

  for (int i = 0; i < 1024; i++) {
    memoria.dados[i] = 0;
  }

  // Exemplo de instrução (LW r1, r2, 10)
  enum OpCode opcode = SW;
  int rd = 1;
  int rs = 2;
  int rt = 3; // Não utilizado neste exemplo
  int imm = 10;

  // Executa a instrução
  executar_instrucao(opcode, rd, rs, rt, imm);

  // Escreve um valor na memória antes de executar a instrução LW
  escrever_memoria(10, 42); // Escreve o valor 42 no endereço de memória 10

  // Imprime o valor do registrador r1 após a execução
  printf("Valor de %s: %d\n", nomes_registradores[rd], registradores[rd]);

  // Imprime o estado dos barramentos
  printf("Estado do Barramento de Leitura: %d\n", ler_barramento());
  printf("Estado do Barramento de Escrita: %d\n",
         registradores[barramento_escrita]);

  // Imprime o conteúdo da memória
  imprimir_memoria();

  return 0;
}
