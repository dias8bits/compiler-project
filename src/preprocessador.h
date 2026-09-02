#ifndef PREPROCESSADOR_H
#define PREPROCESSADOR_H

#include <stdio.h>
#include <stdbool.h>

void preProcessarArquivo(FILE *entrada, FILE *saida);
void removerComentarios(char *linha);
bool removerLinhasVazias(char *linha);
void normalizarEspacosETabulacoes(char * linha);

#endif