#include "lexico.h"

#include <string.h>
#include <stdbool.h>

#define NUM_DIRETIVAS 10
#define NUM_INSTRUCOES 36
#define NUM_REGISTRADORES 31

typedef struct {
    const char *lexema;
    const char *token;
} lexemaToken;

static const lexemaToken DIRETIVAS[] = {
    {".data", "DIR_DATA"},
    {".text", "DIR_TEXT"},
    {".word", "DIR_WORD"},
    {".half", "DIR_HALF"},
    {".byte", "DIR_BYTE"},
    {".space", "DIR_SPACE"},
    {".ascii", "DIR_ASCII"},
    {".asciiz", "DIR_ASCIIZ"},
    {".globl", "DIR_GLOBL"},
    {".align", "DIR_ALIGN"}};

static const lexemaToken INSTRUCOES[] = {
    {"li", "INS_LI"},
    {"la", "INS_LA"},
    {"move", "INS_MOVE"},
    {"lw", "INS_LW"},
    {"sw", "INS_SW"},
    {"lb", "INS_LB"},
    {"sb", "INS_SB"},
    {"lh", "INS_LH"},
    {"sh", "INS_SH"},
    {"add", "INS_ADD"},
    {"addi", "INS_ADDI"},
    {"addu", "INS_ADDU"},
    {"sub", "INS_SUB"},
    {"subu", "INS_SUBU"},
    {"mul", "INS_MUL"},
    {"div", "INS_DIV"},
    {"mflo", "INS_MFLO"},
    {"mfhi", "INS_MFHI"},
    {"and", "INS_AND"},
    {"or", "INS_OR"},
    {"xor", "INS_XOR"},
    {"nor", "INS_NOR"},
    {"sll", "INS_SLL"},
    {"srl", "INS_SRL"},
    {"slt", "INS_SLT"},
    {"beq", "INS_BEQ"},
    {"bne", "INS_BNE"},
    {"blt", "INS_BLT"},
    {"ble", "INS_BLE"},
    {"bgt", "INS_BGT"},
    {"bge", "INS_BGE"},
    {"j", "INS_J"},
    {"jal", "INS_JAL"},
    {"jr", "INS_JR"},
    {"syscall", "INS_SYSCALL"},
    {"nop", "INS_NOP"}};

static const char *REGISTRADORES[] = {
    "$zero",
    "$v0", "$v1",
    "$a0", "$a1", "$a2", "$a3",
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$k0", "$k1",
    "$gp", "$sp", "$fp", "$ra",};

static FILE *arquivoErro;
static int totErros = 0;

void definirArquivoErro(FILE *err) {
    arquivoErro = err;
    totErros = 0;
}

static Simbolo tabelaSimbolos[1024];
static int totalSimbolos = 0;

void analiseLexica(FILE *inFile, FILE *outFile) {
    for (int k = 0; k < NUM_DIRETIVAS; k++) {
        inserirSimbolo((char *) DIRETIVAS[k].lexema, "diretiva", 0, 0);
    }

    for (int k = 0; k < NUM_INSTRUCOES; k++) {
        inserirSimbolo((char *) INSTRUCOES[k].lexema, "instrucao", 0, 0);
    }

    for (int k = 0; k < NUM_REGISTRADORES; k++) {
        inserirSimbolo((char *) REGISTRADORES[k], "registrador", 0, 0);
    }

    char bufferLinha[2048];
    int linha = 1;

    while (fgets(bufferLinha, sizeof(bufferLinha), inFile) != NULL) {
        int coluna = 1;

        for (int i = 0; bufferLinha[i] != '\0'; i++) {
            char c = bufferLinha[i];
            bool reconhecido = false;

            Token token = reconhecerSimbolo(c, linha, coluna);
            if (token.nome[0] != '\0') {
                ehErro(outFile, token);
                reconhecido = true;
            }

            if (c == '.') {
                Token token = reconhecerDiretiva(bufferLinha, i, coluna, linha);
                ehErro(outFile, token);

                if (strcmp(token.nome, "ERRO_DIRETIVA_INVALIDA") != 0) {
                    inserirSimbolo(token.lexema, "diretiva", token.linha, token.coluna);
                }

                int tamLexema = strlen(token.lexema);
                i += tamLexema - 1;
                coluna += tamLexema - 1;
                reconhecido = true;
            }

            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
                Token token = reconhecerInstrucao(bufferLinha, i, coluna, linha);
                ehErro(outFile, token);

                if (strcmp(token.nome, "ID") == 0) {
                    inserirSimbolo(token.lexema, "identificador/rotulo", token.linha, token.coluna);
                } else {
                    inserirSimbolo(token.lexema, "instrucao", token.linha, token.coluna);
                }

                int tamLexema = strlen(token.lexema);
                i += tamLexema - 1;
                coluna += tamLexema - 1;
                reconhecido = true;
            }

            if (c == '$') {
                Token token = reconhecerRegistrador(bufferLinha, i, coluna, linha);
                ehErro(outFile, token);

                if (strcmp(token.nome, "REG") == 0) {
                    inserirSimbolo(token.lexema, "registrador", token.linha, token.coluna);
                }

                int tamLexema = strlen(token.lexema);
                i += tamLexema - 1;
                coluna += tamLexema - 1;
                reconhecido = true;
            }

            if (c == '"') {
                Token token = reconhecerString(bufferLinha, i, coluna, linha);
                ehErro(outFile, token);
                int tamLexema = strlen(token.lexema);
                i += tamLexema - 1;
                coluna += tamLexema - 1;
                reconhecido = true;
            }

            bool ehInicioNumero = (c >= '0' && c <= '9')
                || (c == '-' && (bufferLinha[i + 1] >= '0' && bufferLinha[i + 1] <= '9'));

            if (ehInicioNumero) {
                Token token = reconhecerNumero(bufferLinha, i, coluna, linha);
                ehErro(outFile, token);
                int tamLexema = strlen(token.lexema);
                i += tamLexema - 1;
                coluna += tamLexema - 1;
                reconhecido = true;
            }

            bool ehEspaco = (c == ' ' || c == '\t' || c == '\n' || c == '\r');

            if (!reconhecido && !ehEspaco) {
                char lexemaChar[2] = {c, '\0'};
                fprintf(arquivoErro, "<ERRO_CARACTERE_INVALIDO, %s> %d %d\n", lexemaChar, linha, coluna);
                totErros++;
            }

            coluna++;
        }

        linha++;
    }

    fprintf(outFile, "<TK_EOF, EOF> %d %d\n", linha, 1);

    if (totErros == 0) {
        fprintf(arquivoErro, "nenhum erro lexico encontrado\n");
    }
}

Token reconhecerSimbolo(char c, int linha, int coluna) {
    Token t;
    t.linha = linha;
    t.coluna = coluna;
    t.lexema[0] = c;
    t.lexema[1] = '\0';

    if (c == ',') {
        strcpy(t.nome, "SMB_COM");
    } else if (c == ':') {
        strcpy(t.nome, "SMB_COL");
    } else if (c == '(') {
        strcpy(t.nome, "SMB_OPA");
    } else if (c == ')') {
        strcpy(t.nome, "SMB_CPA");
    } else {
        t.nome[0] = '\0';
    }

    return t;
}

Token reconhecerDiretiva(char *bufferLinha, int i, int coluna, int linha) {
    Token t;
    t.linha = linha;
    t.coluna = coluna;

    int tam = 0;

    for (int j = i; ; j++) {
        char ch = bufferLinha[j];
        bool ehLetra = (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');

        if (ch != '.' && !ehLetra) break;

        t.lexema[tam] = ch;
        tam++;
    }

    t.lexema[tam] = '\0';

    for (int k = 0; k < NUM_DIRETIVAS; k++) {
        if (strcmp(t.lexema, DIRETIVAS[k].lexema) ==0) {
            strcpy(t.nome, DIRETIVAS[k].token);
            return t;
        }
    }

    strcpy(t.nome, "ERRO_DIRETIVA_INVALIDA");
    return t;
}

Token reconhecerInstrucao(char *bufferLinha, int i, int coluna, int linha) {
    Token t;
    t.linha = linha;
    t.coluna = coluna;

    int tam = 0;

    for (int j = i; ; j++) {
        char ch = bufferLinha[j];
        bool ehLetra = (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
        bool ehDigito = (ch >= '0' && ch <= '9');

        if (!ehLetra &&!ehDigito && ch != '_') break;

        t.lexema[tam] = ch;
        tam++;
    }

    t.lexema[tam] = '\0';

    for (int k = 0; k < NUM_INSTRUCOES; k++) {
        if (strcmp(t.lexema, INSTRUCOES[k].lexema) == 0) {
            strcpy(t.nome, INSTRUCOES[k].token);
            return t;
        }
    }

    strcpy(t.nome, "ID");
    return t;
}

Token reconhecerRegistrador(char *bufferLinha, int i, int coluna, int linha) {
    Token t;
    t.linha = linha;
    t.coluna = coluna;

    int tam = 0;

    for (int j = i; ; j++) {
        char ch = bufferLinha[j];
        bool ehLetra = (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
        bool ehDigito = (ch >= '0' && ch <= '9');

        if (ch != '$' && !ehLetra && !ehDigito) break;

        t.lexema[tam] = ch;
        tam++;
    }

    t.lexema[tam] = '\0';

    for (int k = 0; k < NUM_REGISTRADORES; k++) {
        if (strcmp(t.lexema, REGISTRADORES[k]) == 0) {
            strcpy(t.nome, "REG");
            return t;
        }
    }

    bool ehNumerico = (tam > 1);
    for (int k = 1; k < tam; k++) {
        char ch = t.lexema[k];
        if (ch < '0' || ch > '9') {
            ehNumerico = false;
        }
    }

    if (ehNumerico) {
        int valor = 0;
        for (int k = 1; k < tam; k++) {
            valor = valor * 10 + (t.lexema[k] - '0');
        }

        if (valor >= 0 && valor <= 31) {
            strcpy(t.nome, "REG");
            return t;
        }
    }

    strcpy(t.nome, "ERRO_REGISTRADOR_INVALIDO");
    return t;
}


Token reconhecerString(char*bufferLinha, int i, int coluna, int linha) {
    Token t;
    t.linha = linha;
    t.coluna = coluna;

    int tam = 0;
    t.lexema[tam] = bufferLinha[i];
    tam++;

    int j = i + 1;
    bool fechou = false;

    while (bufferLinha[j] != '\0' && bufferLinha[j] != '\n') {

        if (bufferLinha[j] == '\\') {
            t.lexema[tam] = bufferLinha[j];
            tam++;
            j++;

            if (bufferLinha[j] != '\0' && bufferLinha[j] != '\n') {
                t.lexema[tam] = bufferLinha[j];
                tam++;
                j++;
            }
            continue;
        }

        if (bufferLinha[j] == '"') {
            t.lexema[tam] = bufferLinha[j];
            tam++;
            fechou = true;
            break;
        }
        t.lexema[tam] = bufferLinha[j];
        tam++;
        j++;
    }

    t.lexema[tam] ='\0';

    if (fechou) {
        strcpy(t.nome, "STRING");
    } else {
        strcpy(t.nome, "ERRO_STRING_NAO_FECHADA");
    }

    return t;
}

Token reconhecerNumero(char *bufferLinha, int i, int coluna, int linha) {
    Token t;
    t.linha = linha;
    t.coluna = coluna;

    int tam = 0;

    for (int j = i; ; j++) {
        char ch = bufferLinha[j];
        bool ehDigito = (ch >= '0' && ch <= '9');
        bool ehLetraHex = (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');

        if (ch != '-' && ch != 'x' && !ehDigito && !ehLetraHex) break;

        t.lexema[tam] = ch;
        tam++;
    }

    t.lexema[tam] = '\0';

    strcpy(t.nome, "NUM_INT");
    return t;
}

void ehErro(FILE*outFile, Token token) {
    bool ehErro = (token.nome[0] == 'E' && token.nome[1] == 'R' && token.nome[2] == 'R' && token.nome[3] == 'O');

    if (ehErro) {
        fprintf(arquivoErro, "<%s, %s> %d %d\n", token.nome, token.lexema, token.linha, token.coluna);
        totErros++;
    } else {
        fprintf(outFile, "<%s, %s> %d %d\n", token.nome, token.lexema, token.linha, token.coluna);
    }
}

void inserirSimbolo(char *lexema, char *categoria, int linha, int coluna) {
    for (int k = 0; k < totalSimbolos; k++) {
        if (strcmp(tabelaSimbolos[k].lexema, lexema) == 0) {
            if (tabelaSimbolos[k].primeiraLinha == 0 && linha != 0) {
                tabelaSimbolos[k].primeiraLinha = linha;
                tabelaSimbolos[k].primeiraColuna = coluna;
            }
            return;
        }
    }

    if (totalSimbolos >= 1024) {
        return;
    }

    strcpy(tabelaSimbolos[totalSimbolos].lexema, lexema);
    strcpy(tabelaSimbolos[totalSimbolos].categoria, categoria);
    tabelaSimbolos[totalSimbolos].primeiraLinha = linha;
    tabelaSimbolos[totalSimbolos].primeiraColuna = coluna;
    totalSimbolos++;
}

void escreverTabelaSimbolos(FILE *outTs) {
    fprintf(outTs, "LEXEMA,CATEGORIA,LINHA,COLUNA\n");

    for (int k = 0; k < totalSimbolos; k++) {
        fprintf(outTs, "%s,%s,%d,%d\n",
            tabelaSimbolos[k].lexema,
            tabelaSimbolos[k].categoria,
            tabelaSimbolos[k].primeiraLinha,
            tabelaSimbolos[k].primeiraColuna);
    }
}
