#include "memoria.h"

#include "hashtables.h"

#include <stdio.h>
#include <stdlib.h>

void
decodificar(uint32_t instrucao) {
  uint8_t  opcode = instrucao >> 26;
  uint8_t  rd = 0, rs = 0, rt = 0;
  uint16_t imm = 0, extra = 0;
  uint32_t end = 0;

  switch (OpCodeTipo[opcode]) {
  case R:
    rd    = (instrucao >> 11) & 0x1F;
    rs    = (instrucao >> 21) & 0x1F;
    rt    = (instrucao >> 16) & 0x1F;
    extra = instrucao & 0x7FF;
    break;
  case I:
    rt  = (instrucao >> 16) & 0x1F;
    rs  = (instrucao >> 21) & 0x1F;
    imm = instrucao & 0xFFFF;
    break;
  case J: end = instrucao & 0x3FFFFFF; break;
  default: perror("Tipo de instrução não reconhecido"); exit(EXIT_FAILURE);
  }

  printf(
      "Opcode: %u\nRD: %u\nRS: %u\nRT: %u\nIMM: %u\nEXTRA: %u\n"
      "Endereço: %u\n",
      opcode, rd, rs, rt, imm, extra, end);
}
