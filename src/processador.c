#include "processador.h"
#include <string.h>

void preProcessarArquivo(FILE *entrda, FILE *saida) {
    char linha[128];

    while (fgets(linha, sizeof(linha), entrda) != NULL){
        removerComentarios(linha);
        normalizarEspacosETabulacoes(linha);

        if (!removerLinhasVazias(linha)) {
            fputs(linha, saida);
        }
    }
}

void removerComentarios(char *linha) {
    bool ehString = false;

    for (int i = 0; linha[i] != '\0'; i++) {
        if (linha[i] == '"') ehString = !ehString;
        else if (linha[i] == '#' && !ehString) {
            linha[i] = '\0';
            return;
        }
    }
}

bool removerLinhasVazias(char *linha) {
    return strspn(linha, " \t\r\n") == strlen(linha);
}

void normalizarEspacosETabulacoes(char *linha) {
    bool ehString = false;
    char resultado[512];
    size_t escrever = 0;

    for (size_t i = 0; linha[i] != '\0'; i++) {
        char c = linha[i];

        if (c == '"') {
            ehString = !ehString;
            resultado[escrever++] = c;
            continue;
        }

        if (!ehString && (c == ' ' || c == '\t' || c == '\n')) {
            if (escrever > 0 && resultado[escrever - 1] != '\n') {
                resultado[escrever++] = '\n';
            }

            while (linha[i + 1] == ' ' ||
                   linha[i + 1] == '\t' ||
                   linha[i + 1] == '\n') {
                i++;
            }

            continue;
        }

        if (!ehString && strchr(".$,:", c) != NULL) {
            if (escrever > 0 && resultado[escrever - 1] != '\n') {
                resultado[escrever++] = '\n';
            }

            resultado[escrever++] = c;
            resultado[escrever++] = '\n';
            continue;
        }

        resultado[escrever++] = c;
    }

    resultado[escrever] = '\0';
    strcpy(linha, resultado);
}