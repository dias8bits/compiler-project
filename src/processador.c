#include "processador.h"
#include <string.h>

void preProcessarArquivo(FILE *entrda, FILE *saida) {
    char linha[128];

    while (fgets(linha, sizeof(linha), entrda) != NULL){
        removerComentarios(linha);

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