#include "mundo.h"
#include <omp.h>
#include <time.h>

void salvaMundo(Mundo* mundo, int *VAR_PROG){
    FILE *f;
    char filename[50];
    sprintf(filename, "./output/%ld.txt", time(0));
    f = fopen(filename, "w");
    for(int i = 0; i < 6; i++){
        fprintf(f, "%d ", VAR_PROG[i]);
    }
    fprintf(f, "%d\n", VAR_PROG[6]);

    for(int i = 0; i < mundo->elementos.numeroDeObjetos; i++){
        int id = mundo->elementos.ids[i];
        Objeto objeto = mundo->elementos.objetos[id];
        if(objeto.tipo == RAPOSA){
            fprintf(f, "RAPOSA %d %d\n", objeto.x, objeto.y);
        }else if(objeto.tipo == COELHO){
            fprintf(f, "COELHO %d %d\n", objeto.x, objeto.y);
        }else{
            fprintf(f, "ROCHA %d %d\n", objeto.x, objeto.y);
        }
    }
}

void criaMundo(int linhas, int colunas, Mundo *mundo)
{
    mundo->corpo = (int **)malloc(sizeof(int *) * linhas);
    for (int i = 0; i < linhas; i++)
    {
        mundo->corpo[i] = (int *)malloc(sizeof(int) * colunas);
        for (int j = 0; j < colunas; j++)
        {
            mundo->corpo[i][j] = VAZIO;
        }
    }
    mundo->linhas = linhas;
    mundo->colunas = colunas;
    instanciaElementos(linhas, colunas, &mundo->elementos);
}

void insereObjeto(Mundo *mundo, Objeto objeto)
{
    mundo->corpo[objeto.x][objeto.y] = objeto.id;
}

int criaObjeto(Mundo *mundo, int tipo, int x, int y)
{

    for (int i = 0; i < mundo->elementos.numeroDeObjetos; i++)
    {
        int id = mundo->elementos.ids[i];
        if (mundo->elementos.objetos[id].id == -1)
        {
            mundo->elementos.objetos[id].tipo = tipo;
            mundo->elementos.ids[mundo->elementos.numeroDeObjetos] = i;
            mundo->elementos.numeroDeObjetos++;
            mundo->elementos.objetos[id].comida = 0;
            mundo->elementos.objetos[id].proc = 0;
            mundo->elementos.objetos[id].x = x;
            mundo->elementos.objetos[id].y = y;
            mundo->elementos.objetos[id].id = i;
            insereObjeto(mundo, mundo->elementos.objetos[i]);
            return mundo->elementos.objetos[i].id;
        }
    }
}

void removerObjeto(Mundo *mundo, int id)
{
    mundo->elementos.objetos[id].id = VAZIO;
    for(int i = 0; i < mundo->elementos.numeroDeObjetos; i++){
        if(mundo->elementos.ids[i] == id){
            mundo->elementos.ids[i] = mundo->elementos.ids[mundo->elementos.numeroDeObjetos - 1];
            mundo->elementos.ids[mundo->elementos.numeroDeObjetos - 1] = -1;
            break;
        }
    }
    mundo->elementos.numeroDeObjetos--;
}

void imprimeMundo(Mundo mundo)
{
    for (int i = 0; i < mundo.colunas + 2; i++)
    {
        printf("-");
    }
    printf("\n");
    for (int i = 0; i < mundo.linhas; i++)
    {
        for (int j = 0; j < mundo.colunas; j++)
        {
            if (j == 0)
                printf("|");
            switch (mundo.corpo[i][j])
            {
            case VAZIO:
                printf("_");
                break;
            default:
                printf("%d", mundo.elementos.objetos[mundo.corpo[i][j]].tipo);
            }
            if (j == mundo.colunas - 1)
                printf("|");
        }
        printf("\n");
    }
    for (int i = 0; i < mundo.colunas + 2; i++)
    {
        printf("-");
    }
    printf("\n");
}

void iteracao(Mundo *mundo, int *VAR_PROG)
{
        time_t total = 0;
        FILE *f;
        char filename[50];
        sprintf(filename, "./results/%ld.csv", time(0));
        f = fopen(filename, "a");
    for(int geracao = 0; geracao < VAR_PROG[N_GEN]; geracao++){

        time_t t1 , t2;
        
        //Iteracao Coelho
        t1 = clock();
        moveCoelho(mundo, VAR_PROG, geracao);
        t2 = clock();
        total += t2 - t1;
        pa(VAR_PROG, t2 - t1, f, "moveCoelho");
        t1 = clock();
        tornaAdulto(mundo, COELHO);
        t2 = clock();
        total += t2 - t1;
        pa(VAR_PROG, t2 - t1, f, "tornaAdulto1");
        t1 = clock();
        sincronizaMundo(mundo);
        t2 = clock();
        total += t2 - t1;
        pa(VAR_PROG, t2 - t1, f, "sicronizaMundo1");
        
        //Iteracao Raposa
        t1 = clock();
        moveRaposa(mundo, VAR_PROG, geracao);
        t2 = clock();
        total += t2 - t1;
        pa(VAR_PROG, t2 - t1, f, "moveRaposa");
        t1 = clock();
        tornaAdulto(mundo, RAPOSA);
        t2 = clock();
        total += t2 - t1;
        pa(VAR_PROG, t2 - t1, f, "tornaAdulto2");
        t1 = clock();
        sincronizaMundo(mundo);
        t2 = clock();
        total += t2 - t1;
        pa(VAR_PROG, t2 - t1, f, "sincronizaMundo2");
    }

        pa(VAR_PROG, total, f, "iteracao");
    fclose(f);

}

void pa(int *VAR_PROG, time_t t, FILE *f, char label[10])
{
    fprintf(f, "%f,%s", t / (double) CLOCKS_PER_SEC, label);
    for(int i = 0; i < 7; i++){
        fprintf(f, ",%d", VAR_PROG[i]);
    }
    fprintf(f, "\n");
}

void reiniciaMundo(Mundo* mundo){
    int i, j;
    //#pragma omp parallel for private(i, j) num_threads(4)
    for(i = 0; i < mundo->linhas; i++){
        for(j = 0; j < mundo->colunas; j++){
            mundo->corpo[i][j] = VAZIO;
        }
    }
}

void sincronizaMundo(Mundo* mundo){
    reiniciaMundo(mundo);
    for(int i = 0; i < mundo->elementos.numeroDeObjetos; i++){
        int id = mundo->elementos.ids[i];
        int x = mundo->elementos.objetos[id].x;
        int y = mundo->elementos.objetos[id].y;
        if(mundo->elementos.objetos[id].id != VAZIO){
            if(mundo->corpo[x][y] == VAZIO){
                mundo->corpo[x][y] = mundo->elementos.objetos[id].id;
            }
            else{
                int conflito = resolveConflito(mundo->elementos.objetos[mundo->corpo[x][y]], mundo->elementos.objetos[id]);
                if(conflito == mundo->corpo[x][y]){
                    removerObjeto(mundo, mundo->elementos.objetos[id].id);
                }
                else{
                    removerObjeto(mundo, mundo->corpo[x][y]);
                    mundo->corpo[x][y] = conflito;
                }
            }
        }
    }
}

int resolveConflito(Objeto objeto1, Objeto objeto2){
    if(objeto1.tipo == RAPOSA && objeto2.tipo == RAPOSA){
        if(objeto1.comida == objeto2.comida){
            if(objeto1.proc < objeto2.proc){
                return objeto2.id;
            } 
            return objeto1.id;
        }
        if(objeto1.comida < objeto2.comida){
            return objeto1.id;
        }
        return objeto2.id;

    }
    else if(objeto1.tipo == COELHO && objeto1.tipo == COELHO){
        if(objeto1.proc < objeto2.proc){
            return objeto2.id;
        }
        return objeto1.id;
    }
    
    else{
        if(objeto1.tipo == RAPOSA){
            return objeto1.id;
        }

        return objeto2.id;
    }
    
}

void tornaAdulto(Mundo *mundo, int tipo)
{
    for (int i = 0; i < mundo->elementos.numeroDeObjetos; i++)
    {
        int id = mundo->elementos.ids[i];
        if (mundo->elementos.objetos[id].tipo == BEBE)
        {
            mundo->elementos.objetos[id].tipo = tipo;
        }
    }
}

void moveRaposa(Mundo *mundo, int *VAR_PROG, int geracao)
{
    int i;
    #pragma omp parallel for private(i) num_threads(4)
    for (i = 0; i < mundo->elementos.numeroDeObjetos; i++)
    {
        int id = mundo->elementos.ids[i];
        if (mundo->elementos.objetos[id].tipo == RAPOSA)
        {
            moveObjeto(mundo, VAR_PROG, &mundo->elementos.objetos[id], geracao);
        }
    }
}

void moveCoelho(Mundo *mundo, int *VAR_PROG, int geracao)
{
    int i;
    #pragma omp parallel for private(i) num_threads(4)
    for (i = 0; i < mundo->elementos.numeroDeObjetos; i++)
    {
        int id = mundo->elementos.ids[i];
        if (mundo->elementos.objetos[id].tipo == COELHO)
        {
            moveObjeto(mundo, VAR_PROG, &mundo->elementos.objetos[id], geracao);
        }
    }
}

void moveObjeto(Mundo *mundo, int *VAR_PROG, Objeto *objeto, int geracao)
{
    int x = objeto->x;
    int y = objeto->y;
    int posicoes[] = {0, 0, 0, 0};
    int p = 0;

    if (objeto->tipo == RAPOSA)
    {
        p = verificaAdjacente(mundo, objeto, posicoes, COELHO);
        if (p != 0)
        {
            objeto->comida = 0;
        }
        else
        {
            objeto->comida++;
            if(objeto->comida == VAR_PROG[GEN_COMIDA_RAPOSAS]){
                removerObjeto(mundo, objeto->id);
                return;
            }
        }
    }
    if (p == 0)
    {
        p = verificaAdjacente(mundo, objeto, posicoes, VAZIO);
    }

    if (p == 0)
    {
        return;
    }

    reproduzObjeto(mundo, VAR_PROG, objeto);

    int movimento = (geracao + x + y) % p;
    for (int i = 0; i < 4; i++)
    {
        if (movimento == 0 && posicoes[i] == 1)
        {
            if (i == 0)
            {
                if (mundo->corpo[x - 1][y] != VAZIO)
                {
                    removerObjeto(mundo, mundo->corpo[x - 1][y]);
                }
                objeto->x-- ;
            }
            else if (i == 1)
            {
                if (mundo->corpo[x][y + 1] != VAZIO)
                {
                    removerObjeto(mundo, mundo->corpo[x][y + 1]);
                }
                objeto->y++;
            }
            else if (i == 2)
            {
                if (mundo->corpo[x + 1][y] != VAZIO)
                {
                    removerObjeto(mundo, mundo->corpo[x + 1][y]);
                }
                objeto->x++;
            }
            else
            {
                if (mundo->corpo[x][y - 1] != VAZIO)
                {
                    removerObjeto(mundo, mundo->corpo[x][y - 1]);
                }
                objeto->y--;
            }
            break;
        }
        if (posicoes[i] == 1)
        {
            movimento--;
        }
    }
}

int verificaAdjacente(Mundo *mundo, Objeto *objeto, int *posicoes, int tipo)
{
    int x = objeto->x;
    int y = objeto->y;
    int p = 0;
    if (tipo == VAZIO)
    {

        if (x > 0)
        {
            if (mundo->corpo[x - 1][y] == tipo)
            {
                posicoes[0] = 1;
                p++;
            }
        }
        if (y < mundo->colunas - 1)
        {
            if (mundo->corpo[x][y + 1] == tipo)
            {
                posicoes[1] = 1;
                p++;
            }
        }
        if (x < mundo->linhas - 1)
        {
            if (mundo->corpo[x + 1][y] == tipo)
            {
                posicoes[2] = 1;
                p++;
            }
        }
        if (y > 0)
        {
            if (mundo->corpo[x][y - 1] == tipo)
            {
                posicoes[3] = 1;
                p++;
            }
        }
    }
    else
    {
        if (x > 0)
        {
            if (mundo->elementos.objetos[mundo->corpo[x - 1][y]].tipo == tipo)
            {
                posicoes[0] = 1;
                p++;
            }
        }
        if (y < mundo->colunas - 1)
        {
            if (mundo->elementos.objetos[mundo->corpo[x][y + 1]].tipo == tipo)
            {
                posicoes[1] = 1;
                p++;
            }
        }
        if (x < mundo->linhas - 1)
        {
            if (mundo->elementos.objetos[mundo->corpo[x + 1][y]].tipo == tipo)
            {
                posicoes[2] = 1;
                p++;
            }
        }
        if (y > 0)
        {
            if (mundo->elementos.objetos[mundo->corpo[x][y - 1]].tipo == tipo)
            {
                posicoes[3] = 1;
                p++;
            }
        }
    }

    return p;
}

int reproduzObjeto(Mundo *mundo, int *VAR_PROG, Objeto *objeto)
{
    if (objeto->tipo == COELHO)
    {
        if (objeto->proc == VAR_PROG[GEN_PROC_COELHOS])
        {
            objeto->proc = 0;
            criaObjeto(mundo, BEBE, objeto->x, objeto->y);
            return 1;
        }

        else
        {
            objeto->proc++;
            return 0;
        }
    }

    if (objeto->tipo == RAPOSA)
    {
        if (objeto->proc == VAR_PROG[GEN_PROC_RAPOSAS])
        {
            criaObjeto(mundo, BEBE, objeto->x, objeto->y);
            objeto->proc = 0;
            return 1;
        }
        else
        {
            objeto->proc++;
            return 0;
        }
    }

    return 0;
}