#include "preprocessador.h"
#include "lexico.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int validarFileName(const char *nome, const char *ext);
FILE *abrirArquivoEntrada(const char *fileName, const char *modo);
FILE *abrirArquivoSaida(const char *fileName, const char *modo);
void gerarNomePre(const char *lexFileName, char *destino, size_t tamDestino);
void gerarNomeErr(const char *lexFileName, char *destino, size_t tamDestino);

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("ERRO: uso incorreto. Formato esperado:\n");
        printf("./main.exe <arquivo_entrada.asm> <arquivo_saida.lex>\n");
        exit(1);
    }

    char *inFileName = argv[1];
    char *lexFileName = argv[2];

    if (!validarFileName(inFileName, ".asm")) {
        printf("ERRO: arquivo de entrada deve ter extensao .asm\n");
        exit(1);
    }

    if (!validarFileName(lexFileName, ".lex")) {
        printf("ERRO: arquivo de saida deve ter extensao .lex\n");
        exit(1);
    }

    char preFileName[256];
    gerarNomePre(lexFileName, preFileName, sizeof(preFileName));

    char errFileName[256];
    gerarNomeErr(lexFileName, errFileName, sizeof(errFileName));

    FILE *entrada = abrirArquivoEntrada(inFileName, "r");
    FILE *saidaPreProcessador = abrirArquivoSaida(preFileName, "w");

    preProcessarArquivo(entrada, saidaPreProcessador);

    fclose(entrada);
    fclose(saidaPreProcessador);

    FILE *entradaPreProcessador = abrirArquivoEntrada(preFileName, "r");
    FILE *saidaAnaliseLexica = abrirArquivoSaida(lexFileName, "w");
    FILE *saidaErr = abrirArquivoSaida(errFileName, "w");

    definirArquivoErro(saidaErr);
    analiseLexica(entradaPreProcessador, saidaAnaliseLexica);

    fclose(entradaPreProcessador);
    fclose(saidaAnaliseLexica);
    fclose(saidaErr);

    return 0;
}

int validarFileName(const char *nome, const char *ext) {
    size_t lenNome = strlen(nome);
    size_t lenExt = strlen(ext);

    if (lenNome < lenExt) return 0;

    return strcmp(nome + (lenNome - lenExt), ext) == 0;
}

FILE *abrirArquivoEntrada(const char *fileName, const char *modo) {
    FILE *file = fopen(fileName, modo);

    if (file == NULL) {
        printf("ERRO: erro ao abrir arquivo de entrada: %s\n", fileName);
        exit(1);
    }

    return file;
}

FILE *abrirArquivoSaida(const char *fileName, const char *modo) {
    FILE *file = fopen(fileName, modo);

    if (file == NULL) {
        printf("ERRO: erro ao abrir arquivo de saida: %s\n", fileName);
        exit(1);
    }

    return file;
}

void gerarNomePre(const char *lexFileName, char *destino, size_t tamDestino) {
    strncpy(destino, lexFileName, tamDestino - 1);
    destino[tamDestino - 1] = '\0';

    size_t len = strlen(destino);
    destino[len - 3] = 'p';
    destino[len - 2] = 'r';
    destino[len - 1] = 'e';
}

void gerarNomeErr(const char *lexFileName, char *destino, size_t tamDestino) {
    strncpy(destino, lexFileName, tamDestino - 1);
    destino[tamDestino - 1] = '\0';

    size_t len = strlen(destino);
    destino[len - 3] = 'e';
    destino[len - 2] = 'r';
    destino[len - 1] = 'r';
}