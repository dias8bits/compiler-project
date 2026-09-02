#include "preprocessador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int validarFileName(const char *nome, const char *ext);
FILE *abrirArquivoEntrada(const char *fileName, const char *modo);
FILE *abrirArquivoSaida(const char *fileName, const char *modo);

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("ERRO: uso incorreto. Formato esperado:\n");
        printf("./main.exe <arquivo_entrada.asm> <arquivo_saida.pre>\n");
        exit(1);
    }

    char *inFileName = argv[1];
    char *outFileName = argv[2];

    if (!validarFileName(inFileName, ".asm")) {
        printf("ERRO: arquivo de entrada deve ter extensao .asm\n");
        exit(1);
    }

    if (!validarFileName(outFileName, ".pre")) {
        printf("ERRO: arquivo de saida deve ter extensao .pre\n");
        exit(1);
    }

    FILE *entrada = abrirArquivoEntrada(inFileName, "r");
    FILE *saida = abrirArquivoSaida(outFileName, "w");

    preProcessarArquivo(entrada, saida);

    fclose(entrada);
    fclose(saida);

    return 0;
}

int validarFileName(const char *nome, const char *ext) {
    size_t lenNome = strlen(nome);
    size_t lenExt = strlen(ext);

    if (lenNome < lenExt) return 0;

    return strcmp(nome + (lenNome - lenExt), ext) == 0;
}

FILE *abrirArquivoEntrada(const char *fileName, const char *modo) {
    const char *diretorio = "../inFile/";

    char caminho[256] = "";

    strncat(caminho, diretorio, sizeof(caminho) - 1);
    strncat(caminho, fileName, sizeof(caminho) - strlen(caminho) - 1);

    FILE *file = fopen(caminho, modo);

    if (file == NULL) {
        printf("ERRO: erro ao abrir arquivo de entrada: %s\n", fileName);
        exit(1);
    }

    return file;
}

FILE *abrirArquivoSaida(const char *fileName, const char *modo) {
    const char *diretorio = "../outFile/";

    char caminho[256] = "";

    strncat(caminho, diretorio, sizeof(caminho) - 1);
    strncat(caminho, fileName, sizeof(caminho) - strlen(caminho) - 1);

    FILE *file = fopen(caminho, modo);

    if (file == NULL) {
        printf("ERRO: erro ao abrir arquivo de saida: %s\n", fileName);
        exit(1);
    }

    return file;
}