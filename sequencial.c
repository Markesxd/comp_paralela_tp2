#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mundo.h"

int main(void){
    int VAR_PROG[7];
    char dir[10];
    
    for(int i = 0; i < 7; i++){
        scanf("%s", dir);
        VAR_PROG[i] = atoi(dir);
    }
    Mundo mundo;
    criaMundo(VAR_PROG[L], VAR_PROG[C], &mundo);

    for(int i = 0; i < VAR_PROG[N]; i++){
        char nome[7], x[10], y[10];
        scanf("%s", nome);
        scanf("%s", x);
        scanf("%s", y);
        int tipo;
         if (strcmp(nome, "RAPOSA") == 0)
            {
                tipo = RAPOSA;
            }
            else if (strcmp(nome, "ROCHA") == 0)
            {
                tipo = ROCHA;
            }
            else if (strcmp(nome, "COELHO") == 0)
            {
                tipo = COELHO;
            }
        criaObjeto(&mundo, tipo, atoi(x), atoi(y));
    }

    for(int i = 0; i < VAR_PROG[N_GEN]; i++){
        imprimeMundo(mundo);
        iteracao(&mundo, VAR_PROG, i);
    }


    return 0;
}