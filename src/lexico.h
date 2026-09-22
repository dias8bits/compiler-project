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
void definirArquivoErro(FILE *err);
void ehErro(FILE*outFile, Token token); 
void inserirSimbolo(char *lexema, char *categoria, int linha, int coluna);
void escreverTabelaSimbolos(FILE *outTs);
Token reconhecerSimbolo(char c, int linha, int coluna);
Token reconhecerDiretiva(char *bufferLinha, int i, int coluna, int linha);
Token reconhecerInstrucao(char *bufferLinha, int i, int coluna, int linha);
Token reconhecerRegistrador(char *bufferLinha, int i, int coluna, int linha);
Token reconhecerString(char *bufferLinha, int i, int coluna, int linha);
Token reconhecerNumero(char *bufferLinha, int i, int coluna, int linha);

#endif