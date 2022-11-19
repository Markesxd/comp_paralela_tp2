#include "mundo.h"
#include <omp.h>

void salvaMundo(Mundo* mundo, int *VAR_PROG){
    for(int i = 0; i < 6; i++){
        printf("%d ", VAR_PROG[i]);
    }
    printf("%d\n", VAR_PROG[6]);

    for(int i = 0; i < mundo->linhas * mundo->colunas; i++){
        Objeto objeto = mundo->elementos.objetos[i];
        if(objeto.id != VAZIO){
            if(objeto.tipo == RAPOSA){
                printf("RAPOSA %d %d\n", objeto.x, objeto.y);
            }else if(objeto.tipo == COELHO){
                printf("COELHO %d %d\n", objeto.x, objeto.y);
            }else{
                printf("ROCHA %d %d\n", objeto.x, objeto.y);
            }
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

    for (int i = 0; i < mundo->linhas * mundo->colunas; i++)
    {
        if (mundo->elementos.objetos[i].id == -1)
        {
            mundo->elementos.objetos[i].tipo = tipo;
            mundo->elementos.numeroDeObjetos++;
            mundo->elementos.objetos[i].comida = 0;
            mundo->elementos.objetos[i].proc = 0;
            mundo->elementos.objetos[i].x = x;
            mundo->elementos.objetos[i].y = y;
            mundo->elementos.objetos[i].id = i;
            insereObjeto(mundo, mundo->elementos.objetos[i]);
            return mundo->elementos.objetos[i].id;
        }
    }
}

void removerObjeto(Mundo *mundo, int id)
{
    mundo->elementos.objetos[id].id = VAZIO;
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

void iteracao(Mundo *mundo, int *VAR_PROG, int geracao)
{
    //Iteracao Coelho
    moveCoelho(mundo, VAR_PROG, geracao);
    tornaAdulto(mundo, COELHO);
    sincronizaMundo(mundo);
    
    //Iteracao Raposa
    moveRaposa(mundo, VAR_PROG, geracao);
    tornaAdulto(mundo, RAPOSA);
    sincronizaMundo(mundo);

}

void reiniciaMundo(Mundo* mundo){
    #pragma omp parallel for num_threads(2)
    for(int i = 0; i < mundo->linhas; i++){
        for(int j = 0; j < mundo->colunas; j++){
            mundo->corpo[i][j] = VAZIO;
        }
    }
}

void sincronizaMundo(Mundo* mundo){
    reiniciaMundo(mundo);
    for(int i = 0; i < mundo->linhas * mundo->colunas; i++){
        int x = mundo->elementos.objetos[i].x;
        int y = mundo->elementos.objetos[i].y;
        if(mundo->elementos.objetos[i].id != VAZIO){
            if(mundo->corpo[x][y] == VAZIO){
                mundo->corpo[x][y] = mundo->elementos.objetos[i].id;
            }
            else{
                int conflito = resolveConflito(mundo->elementos.objetos[mundo->corpo[x][y]], mundo->elementos.objetos[i]);
                if(conflito == mundo->corpo[x][y]){
                    removerObjeto(mundo, mundo->elementos.objetos[i].id);
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
    for (int i = 0; i < mundo->linhas * mundo->colunas; i++)
    {
        if (mundo->elementos.objetos[i].id != -1 && mundo->elementos.objetos[i].tipo == BEBE)
        {
            mundo->elementos.objetos[i].tipo = tipo;
        }
    }
}

void moveRaposa(Mundo *mundo, int *VAR_PROG, int geracao)
{
    #pragma omp parallel for num_threads(2)
    for (int i = 0; i < mundo->colunas * mundo->linhas; i++)
    {
        if (mundo->elementos.objetos[i].id != -1 && mundo->elementos.objetos[i].tipo == RAPOSA)
        {
            moveObjeto(mundo, VAR_PROG, &mundo->elementos.objetos[i], geracao);
        }
    }
}

void moveCoelho(Mundo *mundo, int *VAR_PROG, int geracao)
{

    #pragma omp parallel for num_threads(2)
    for (int i = 0; i < mundo->colunas * mundo->linhas; i++)
    {
        if (mundo->elementos.objetos[i].id != -1 && mundo->elementos.objetos[i].tipo == COELHO)
        {
            moveObjeto(mundo, VAR_PROG, &mundo->elementos.objetos[i], geracao);
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