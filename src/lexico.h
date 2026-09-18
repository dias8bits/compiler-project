#ifndef LEXICO_H
#define LEXICO_H

#include <stdio.h>

typedef struct {
    char nome[50];
    char lexema[100];
    int linha;
    int coluna;
} Token;

typedef struct {
    char lexema[100];
    char categoria[30];
    int primeiraLinha;
    int primeiraColuna;
} Simbolo;

void analiseLexica(FILE *in, FILE *out);

#endif