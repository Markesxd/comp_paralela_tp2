#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define VAZIO -1
#define ROCHA 0
#define COELHO 1
#define RAPOSA 2
#define BEBE 3

typedef struct {
    int id;
    int x, y;
    int tipo;
    int comida;
    int proc;
} Objeto;

typedef struct{
    int numeroDeObjetos;
    int* ids;
    Objeto *objetos;
} Elementos;

void instanciaElementos(int linha, int coluna, Elementos *elementos);