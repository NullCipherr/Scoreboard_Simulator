#pragma once

#include <stdint.h>

typedef unsigned int                Registrador;
typedef struct _cpu_specs           CPU_Specs;
typedef struct _scoreboard          Scoreboard;
typedef struct _banco_registradores Banco_Registradores;

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
};

struct _banco_registradores {
  Registrador r0;
  Registrador r1;
  Registrador r2;
  Registrador r3;
  Registrador r4;
  Registrador r5;
  Registrador r6;
  Registrador r7;
  Registrador r8;
  Registrador r9;
  Registrador r10;
  Registrador r11;
  Registrador r12;
  Registrador r13;
  Registrador r14;
  Registrador r15;
  Registrador r16;
  Registrador r17;
  Registrador r18;
  Registrador r19;
  Registrador r20;
  Registrador r21;
  Registrador r22;
  Registrador r23;
  Registrador r24;
  Registrador r25;
  Registrador r26;
  Registrador r27;
  Registrador r28;
  Registrador r29;
  Registrador r30;
  Registrador r31;
};

struct _scoreboard {
  int a;
};
