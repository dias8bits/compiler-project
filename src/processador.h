#ifndef PROCESSADOR_H
#define PROCESSADOR_H

#include <stdio.h>
#include <stdbool.h>

void preProcessarArquivo(FILE *entrada, FILE *saida);
void removerComentarios(char *linha);
bool removerLinhasVazias(char *linha);
void normalizarEspacosETabulacoes();

#endif