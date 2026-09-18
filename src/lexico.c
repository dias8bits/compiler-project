#include "lexico.h"

typedef struct {
    const char *lexema;
    const char *token;
} lexemaToken;

static const lexemaToken DIRETIVAS[] = {
    {".data", "DIR_DATA"},
    {".text", "DIR_TEXT"},
    {".word", "DIR_WORD"},
    {".half", "DIR_HALF"},
    {".byte", "DIR_BYTE"},
    {".space", "DIR_SPACE"},
    {".ascii", "DIR_ASCII"},
    {".asciiz", "DIR_ASCIIZ"},
    {".globl", "DIR_GLOBL"},
    {".align", "DIR_ALIGN"}};

static const lexemaToken INSTRUCOES[] = {
    {"li", "INS_LI"},
    {"la", "INS_LA"},
    {"move", "INS_MOVE"},
    {"lw", "INS_LW"},
    {"sw", "INS_SW"},
    {"lb", "INS_LB"},
    {"sb", "INS_SB"},
    {"lh", "INS_LH"},
    {"sh", "INS_SH"},
    {"add", "INS_ADD"},
    {"addi", "INS_ADDI"},
    {"addu", "INS_ADDU"},
    {"sub", "INS_SUB"},
    {"subu", "INS_SUBU"},
    {"mul", "INS_MUL"},
    {"div", "INS_DIV"},
    {"mflo", "INS_MFLO"},
    {"mfhi", "INS_MFHI"},
    {"and", "INS_AND"},
    {"or", "INS_OR"},
    {"xor", "INS_XOR"},
    {"nor", "INS_NOR"},
    {"sll", "INS_SLL"},
    {"srl", "INS_SRL"},
    {"slt", "INS_SLT"},
    {"beq", "INS_BEQ"},
    {"bne", "INS_BNE"},
    {"blt", "INS_BLT"},
    {"ble", "INS_BLE"},
    {"bgt", "INS_BGT"},
    {"bge", "INS_BGE"},
    {"j", "INS_J"},
    {"jal", "INS_JAL"},
    {"jr", "INS_JR"},
    {"syscall", "INS_SYSCALL"},
    {"nop", "INS_NOP"}};

static const char *REGISTRADORES[] = {
    "$zero",
    "$v0", "$v1",
    "$a0", "$a1", "$a2", "$a3",
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$k0", "$k1",
    "$gp", "$sp", "$fp", "$ra",};
