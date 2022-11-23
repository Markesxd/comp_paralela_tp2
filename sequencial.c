#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mundo.h"
#include <omp.h>
#include <time.h>

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
    
    time_t t1 = clock();
        iteracao(&mundo, VAR_PROG);
    time_t t2 = clock();
    printf("time: %f\n", (t2 - t1) / (double) CLOCKS_PER_SEC);
    FILE *f;
    f = fopen("./result.csv" , "a");
    fprintf(f, "%f,main_prl_v8",  ((t2 - t1) / (double) CLOCKS_PER_SEC));
    for(int i = 0; i < 7; i++){
        fprintf(f, ",%d", VAR_PROG[i]);
    }
    fprintf(f, "\n");
    fclose(f);

    salvaMundo(&mundo, VAR_PROG);

    return 0;
}