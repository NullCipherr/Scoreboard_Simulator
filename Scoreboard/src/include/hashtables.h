#pragma once

/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf opcodehash  */
/* Computed positions: -k'1-2' */

#if !(                                                                         \
    (' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) && ('%' == 37)    \
    && ('&' == 38) && ('\'' == 39) && ('(' == 40) && (')' == 41)               \
    && ('*' == 42) && ('+' == 43) && (',' == 44) && ('-' == 45) && ('.' == 46) \
    && ('/' == 47) && ('0' == 48) && ('1' == 49) && ('2' == 50) && ('3' == 51) \
    && ('4' == 52) && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
    && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) && ('=' == 61) \
    && ('>' == 62) && ('?' == 63) && ('A' == 65) && ('B' == 66) && ('C' == 67) \
    && ('D' == 68) && ('E' == 69) && ('F' == 70) && ('G' == 71) && ('H' == 72) \
    && ('I' == 73) && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
    && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) && ('R' == 82) \
    && ('S' == 83) && ('T' == 84) && ('U' == 85) && ('V' == 86) && ('W' == 87) \
    && ('X' == 88) && ('Y' == 89) && ('Z' == 90) && ('[' == 91)                \
    && ('\\' == 92) && (']' == 93) && ('^' == 94) && ('_' == 95)               \
    && ('a' == 97) && ('b' == 98) && ('c' == 99) && ('d' == 100)               \
    && ('e' == 101) && ('f' == 102) && ('g' == 103) && ('h' == 104)            \
    && ('i' == 105) && ('j' == 106) && ('k' == 107) && ('l' == 108)            \
    && ('m' == 109) && ('n' == 110) && ('o' == 111) && ('p' == 112)            \
    && ('q' == 113) && ('r' == 114) && ('s' == 115) && ('t' == 116)            \
    && ('u' == 117) && ('v' == 118) && ('w' == 119) && ('x' == 120)            \
    && ('y' == 121) && ('z' == 122) && ('{' == 123) && ('|' == 124)            \
    && ('}' == 125) && ('~' == 126))
  /* The character set is not based on ISO-646.  */
  #error                                                                       \
      "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "opcodehash"

#include "memoria.h"
#line 15 "opcodehash"

struct OpCodeMap {
  char   *operacao;
  uint8_t opcode;
  tipo    t;
};

#include <string.h>
/* maximum key range = 38, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
  #ifdef __cplusplus
inline
  #endif
#endif
    static unsigned int
    hash_operacao(register const char *str, register size_t len) {
  static unsigned char asso_values[] = {
    39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
    39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
    39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
    39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
    39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
    39, 39, 5,  10, 39, 0,  15, 39, 25, 39, 8,  0,  39, 10, 3,  5,  25, 39, 39,
    0,  0,  39, 0,  39, 0,  0,  39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
    39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
    39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
    39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
    39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
    39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
    39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
    39, 39, 39, 39, 39, 39, 39, 39, 39
  };
  register unsigned int hval = len;

  switch (hval) {
  default: hval += asso_values[(unsigned char) str[1]];
  /*FALLTHROUGH*/
  case 1: hval += asso_values[(unsigned char) str[0]]; break;
  }
  return hval;
}

static inline struct OpCodeMap
encontra_operacao(register const char *str, register size_t len) {
  enum {
    OP_TOTAL_KEYWORDS  = 17,
    OP_MIN_WORD_LENGTH = 1,
    OP_MAX_WORD_LENGTH = 4,
    OP_MIN_HASH_VALUE  = 1,
    OP_MAX_HASH_VALUE  = 38
  };

  struct OpCodeMap operacoes[] = {
#line 30 "opcodehash"
    {   "j", 0x0D, J},
#line 32 "opcodehash"
    {  "sw", 0x0F, I},
#line 19 "opcodehash"
    { "sub", 0x02, R},
#line 20 "opcodehash"
    {"subi", 0x03, I},
#line 21 "opcodehash"
    { "mul", 0x04, R},
#line 17 "opcodehash"
    { "add", 0x00, R},
#line 18 "opcodehash"
    {"addi", 0x01, I},
#line 22 "opcodehash"
    { "div", 0x05, R},
#line 31 "opcodehash"
    {  "lw", 0x0E, I},
#line 23 "opcodehash"
    { "and", 0x06, R},
#line 29 "opcodehash"
    { "bne", 0x0C, I},
#line 33 "opcodehash"
    {"exit", 0x10, J},
#line 26 "opcodehash"
    { "blt", 0x09, I},
#line 24 "opcodehash"
    {  "or", 0x07, R},
#line 28 "opcodehash"
    { "beq", 0x0B, I},
#line 25 "opcodehash"
    { "not", 0x08, R},
#line 27 "opcodehash"
    { "bgt", 0x0A, I}
  };

  if (len <= OP_MAX_WORD_LENGTH && len >= OP_MIN_WORD_LENGTH) {
    register unsigned int key = hash_operacao(str, len);

    if (key <= OP_MAX_HASH_VALUE && key >= OP_MIN_HASH_VALUE) {
      register struct OpCodeMap resword;

      switch (key - 1) {
      case 0: resword = operacoes[0]; goto compare;
      case 1: resword = operacoes[1]; goto compare;
      case 2: resword = operacoes[2]; goto compare;
      case 3: resword = operacoes[3]; goto compare;
      case 5: resword = operacoes[4]; goto compare;
      case 7: resword = operacoes[5]; goto compare;
      case 8: resword = operacoes[6]; goto compare;
      case 10: resword = operacoes[7]; goto compare;
      case 11: resword = operacoes[8]; goto compare;
      case 12: resword = operacoes[9]; goto compare;
      case 17: resword = operacoes[10]; goto compare;
      case 18: resword = operacoes[11]; goto compare;
      case 22: resword = operacoes[12]; goto compare;
      case 26: resword = operacoes[13]; goto compare;
      case 27: resword = operacoes[14]; goto compare;
      case 32: resword = operacoes[15]; goto compare;
      case 37: resword = operacoes[16]; goto compare;
      }
      return (struct OpCodeMap){ 0 };
compare : {
  register const char *s = resword.operacao;

  if (*str == *s && !strncmp(str + 1, s + 1, len - 1) && s[len] == '\0') {
    return resword;
  }
}
    }
  }
  return (struct OpCodeMap){ 0 };
}

/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf reghash  */
/* Computed positions: -k'2-3' */

#if !(                                                                         \
    (' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) && ('%' == 37)    \
    && ('&' == 38) && ('\'' == 39) && ('(' == 40) && (')' == 41)               \
    && ('*' == 42) && ('+' == 43) && (',' == 44) && ('-' == 45) && ('.' == 46) \
    && ('/' == 47) && ('0' == 48) && ('1' == 49) && ('2' == 50) && ('3' == 51) \
    && ('4' == 52) && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
    && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) && ('=' == 61) \
    && ('>' == 62) && ('?' == 63) && ('A' == 65) && ('B' == 66) && ('C' == 67) \
    && ('D' == 68) && ('E' == 69) && ('F' == 70) && ('G' == 71) && ('H' == 72) \
    && ('I' == 73) && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
    && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) && ('R' == 82) \
    && ('S' == 83) && ('T' == 84) && ('U' == 85) && ('V' == 86) && ('W' == 87) \
    && ('X' == 88) && ('Y' == 89) && ('Z' == 90) && ('[' == 91)                \
    && ('\\' == 92) && (']' == 93) && ('^' == 94) && ('_' == 95)               \
    && ('a' == 97) && ('b' == 98) && ('c' == 99) && ('d' == 100)               \
    && ('e' == 101) && ('f' == 102) && ('g' == 103) && ('h' == 104)            \
    && ('i' == 105) && ('j' == 106) && ('k' == 107) && ('l' == 108)            \
    && ('m' == 109) && ('n' == 110) && ('o' == 111) && ('p' == 112)            \
    && ('q' == 113) && ('r' == 114) && ('s' == 115) && ('t' == 116)            \
    && ('u' == 117) && ('v' == 118) && ('w' == 119) && ('x' == 120)            \
    && ('y' == 121) && ('z' == 122) && ('{' == 123) && ('|' == 124)            \
    && ('}' == 125) && ('~' == 126))
  /* The character set is not based on ISO-646.  */
  #error                                                                       \
      "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 12 "reghash"

struct RegHashMap {
  char   *reg;
  uint8_t identificador;
};

/* maximum key range = 67, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
  #ifdef __cplusplus
inline
  #endif
#endif
    static unsigned int
    hash_reg(register const char *str, register size_t len) {
  static const unsigned char asso_values[] = {
    69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69,
    69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69,
    69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 15, 5,  0,  10, 25, 12, 1,  60,
    50, 40, 36, 26, 16, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69,
    69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69,
    69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69,
    69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69,
    69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69,
    69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69,
    69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69,
    69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69,
    69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69,
    69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69,
    69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69
  };
  register unsigned int hval = len;

  switch (hval) {
  default: hval += asso_values[(unsigned char) str[2] + 4];
  /*FALLTHROUGH*/
  case 2: hval += asso_values[(unsigned char) str[1] + 1]; break;
  }
  return hval;
}

static inline uint8_t
encontra_reg(register const char *str, register size_t len) {
  enum {
    REG_TOTAL_KEYWORDS  = 32,
    REG_MIN_WORD_LENGTH = 2,
    REG_MAX_WORD_LENGTH = 3,
    REG_MIN_HASH_VALUE  = 2,
    REG_MAX_HASH_VALUE  = 68
  };

  static const struct RegHashMap reg_array[] = {
#line 16 "reghash"
    { "r2", 0x02},
#line 20 "reghash"
    { "r6", 0x06},
#line 37 "reghash"
    {"r23", 0x17},
#line 15 "reghash"
    { "r1", 0x01},
#line 27 "reghash"
    {"r13", 0x0D},
#line 17 "reghash"
    { "r3", 0x03},
#line 34 "reghash"
    {"r20", 0x14},
#line 19 "reghash"
    { "r5", 0x05},
#line 36 "reghash"
    {"r22", 0x16},
#line 14 "reghash"
    { "r0", 0x00},
#line 24 "reghash"
    {"r10", 0x0A},
#line 43 "reghash"
    {"r29", 0x1D},
#line 26 "reghash"
    {"r12", 0x0C},
#line 44 "reghash"
    {"r30", 0x1E},
#line 33 "reghash"
    {"r19", 0x13},
#line 18 "reghash"
    { "r4", 0x04},
#line 35 "reghash"
    {"r21", 0x15},
#line 42 "reghash"
    {"r28", 0x1C},
#line 25 "reghash"
    {"r11", 0x0B},
#line 32 "reghash"
    {"r18", 0x12},
#line 45 "reghash"
    {"r31", 0x1F},
#line 41 "reghash"
    {"r27", 0x1B},
#line 23 "reghash"
    { "r9", 0x09},
#line 40 "reghash"
    {"r26", 0x1A},
#line 31 "reghash"
    {"r17", 0x11},
#line 30 "reghash"
    {"r16", 0x10},
#line 22 "reghash"
    { "r8", 0x08},
#line 39 "reghash"
    {"r25", 0x19},
#line 29 "reghash"
    {"r15", 0x0F},
#line 21 "reghash"
    { "r7", 0x07},
#line 38 "reghash"
    {"r24", 0x18},
#line 28 "reghash"
    {"r14", 0x0E}
  };

  if (len <= REG_MAX_WORD_LENGTH && len >= REG_MIN_WORD_LENGTH) {
    register unsigned int key = hash_reg(str, len);

    if (key <= REG_MAX_HASH_VALUE && key >= REG_MIN_HASH_VALUE) {
      register const struct RegHashMap *resword;

      switch (key - 2) {
      case 0: resword = &reg_array[0]; goto compare;
      case 1: resword = &reg_array[1]; goto compare;
      case 2: resword = &reg_array[2]; goto compare;
      case 5: resword = &reg_array[3]; goto compare;
      case 7: resword = &reg_array[4]; goto compare;
      case 10: resword = &reg_array[5]; goto compare;
      case 11: resword = &reg_array[6]; goto compare;
      case 12: resword = &reg_array[7]; goto compare;
      case 13: resword = &reg_array[8]; goto compare;
      case 15: resword = &reg_array[9]; goto compare;
      case 16: resword = &reg_array[10]; goto compare;
      case 17: resword = &reg_array[11]; goto compare;
      case 18: resword = &reg_array[12]; goto compare;
      case 21: resword = &reg_array[13]; goto compare;
      case 22: resword = &reg_array[14]; goto compare;
      case 25: resword = &reg_array[15]; goto compare;
      case 26: resword = &reg_array[16]; goto compare;
      case 27: resword = &reg_array[17]; goto compare;
      case 31: resword = &reg_array[18]; goto compare;
      case 32: resword = &reg_array[19]; goto compare;
      case 36: resword = &reg_array[20]; goto compare;
      case 37: resword = &reg_array[21]; goto compare;
      case 40: resword = &reg_array[22]; goto compare;
      case 41: resword = &reg_array[23]; goto compare;
      case 42: resword = &reg_array[24]; goto compare;
      case 46: resword = &reg_array[25]; goto compare;
      case 50: resword = &reg_array[26]; goto compare;
      case 51: resword = &reg_array[27]; goto compare;
      case 56: resword = &reg_array[28]; goto compare;
      case 60: resword = &reg_array[29]; goto compare;
      case 61: resword = &reg_array[30]; goto compare;
      case 66: resword = &reg_array[31]; goto compare;
      }
      return 0;
compare : {
  register const char *s = resword->reg;

  if (*str == *s && !strncmp(str + 1, s + 1, len - 1) && s[len] == '\0') {
    return resword->identificador;
  }
}
    }
  }
  return 0;
}

static tipo OpCodeTipo[] = {
  R, I, R, I, R, R, R, R, R, I, I, I, I, J, I, I, J
};
