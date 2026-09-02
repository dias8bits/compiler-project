#include "preprocessador.h"
#include <string.h>

void preProcessarArquivo(FILE *entrda, FILE *saida) {
    char linha[128];
    bool ehEspaco = true;

    while (fgets(linha, sizeof(linha), entrda) != NULL){
        removerComentarios(linha);
        normalizarEspacosETabulacoes(linha);

        if (!removerLinhasVazias(linha)) {
            if(!ehEspaco){
                fputc('\n', saida);
            }
            fputs(linha, saida);
            ehEspaco = false;
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

    for (int i = 0; linha[i] != '\0'; i++) {
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

    resultado[e] = '\0';

    strcpy(linha, resultado);
}