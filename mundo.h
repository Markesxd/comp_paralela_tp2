#include "objeto.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define GEN_PROC_COELHOS 0
#define GEN_PROC_RAPOSAS 1
#define GEN_COMIDA_RAPOSAS 2
#define N_GEN 3
#define L 4
#define C 5
#define N 6

typedef struct {
    int** corpo;
    Elementos elementos;
    int linhas, colunas;
} Mundo; 


void criaMundo(int linhas, int colunas, Mundo *mundo);
void insereObjeto(Mundo* mundo, Objeto objeto);
void imprimeMundo(Mundo mundo);
void iteracao(Mundo *mundo, int *VAR_PROG, int geracao);
int reproduzObjeto(Mundo *mundo, int *VAR_PROG, Objeto *objeto);
int criaObjeto(Mundo *mundo, int tipo, int x, int y);
void removerObjeto(Mundo *mundo, int id);
void moveCoelho(Mundo *mundo, int *VAR_PROG, int geracao);
void moveObjeto(Mundo *mundo, int *VAR_PROG, Objeto* objeto, int geracao);
void tornaAdulto(Mundo* mundo, int tipo);
int verificaAdjacente(Mundo *mundo, Objeto* objeto, int *posicoes, int tipo);
void moveRaposa(Mundo *mundo, int *VAR_PROG, int geracao);
void reiniciaMundo(Mundo* mundo);
void sincronizaMundo(Mundo* mundo);
void salvaMundo(Mundo* mundo, int *VAR_PROG);
int resolveConflito(Objeto objeto1, Objeto objeto2);
