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
    int e = 0;

    for (int i = 0; linha[i] != '\0' && e < (int)sizeof(resultado) - 1; i++) {
        char c = linha[i];

        if (c == '\r' || c == '\n') {
            continue;
        }

        if (!ehString && (c == ' ' || c == '\t')) {
            if (e > 0 && resultado[e - 1] != ' ') {
                resultado[e++] = ' ';
            }
            continue;
        }

        if (c == '"') {
            ehString = !ehString;
        }

        resultado[e++] = c;
    }

    while (e > 0 && resultado[e - 1] == ' ') {
        e--;
    }

    if (e > 0) {
        resultado[e++] = '\n';
    }
    resultado[e] = '\0';

    strcpy(linha, resultado);
}