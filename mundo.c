#include "mundo.h"

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

/*void criaBebe(Mundo *mundo, int tipo,int x, int y){
    for(int i = 0; i < mundo->linhas * mundo->colunas; i++){
        if(mundo->elementos.bebes[i].id == -1){
            mundo->elementos.bebes[i].x = x;
            mundo->elementos.bebes[i].y = y;
            mundo->elementos.bebes[i].tipo = tipo;
            mundo->elementos.bebes[i].id = 0;
            return;
        }
    }
}*/

/*void insereBebes(Mundo *mundo){
    for(int i = 0; i < mundo->linhas * mundo->colunas; i++){
        if(mundo->elementos.bebes[i].id == 0){
            if(mundo->corpo[mundo->elementos.bebes[i].x][mundo->elementos.bebes[i].y] == VAZIO){
                criaObjeto(mundo, mundo->elementos.bebes[i].tipo, mundo->elementos.bebes[i].x, mundo->elementos.bebes[i].y);
            }
            else{
                if(mundo->elementos.bebes[i].tipo == )
            }
        }
    }
}*/

void removerObjeto(Mundo *mundo, int id)
{
    mundo->elementos.objetos[id].id = -1;
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
    moveCoelho(mundo, VAR_PROG, geracao);
    tornaAdulto(mundo, COELHO);
    moveRaposa(mundo, VAR_PROG, geracao);
    tornaAdulto(mundo, RAPOSA);

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
                mundo->corpo[x][y] = VAZIO;
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
            // p = 3
            // movimeto = 0;
            // [1, 0, 0, 1]
            if (i == 0)
            {
                if (mundo->corpo[x - 1][y] != VAZIO)
                {
                    removerObjeto(mundo, mundo->corpo[x - 1][y]);
                }
                mundo->corpo[x - 1][y] = objeto->id;
                objeto->x = x - 1;
                printf("id: %d Moveu para norte\n", objeto->id);
            }
            else if (i == 1)
            {
                if (mundo->corpo[x][y + 1] != VAZIO)
                {
                    removerObjeto(mundo, mundo->corpo[x][y + 1]);
                }
                mundo->corpo[x][y + 1] = objeto->id;
                printf("id: %d Moveu para leste\n", objeto->id);
                objeto->y = y + 1;
            }
            else if (i == 2)
            {
                if (mundo->corpo[x + 1][y] != VAZIO)
                {
                    removerObjeto(mundo, mundo->corpo[x + 1][y]);
                }
                mundo->corpo[x + 1][y] = objeto->id;
                printf("id: %d Moveu para sul\n", objeto->id);
                objeto->x = x + 1;
            }
            else
            {
                if (mundo->corpo[x][y - 1] != VAZIO)
                {
                    removerObjeto(mundo, mundo->corpo[x][y - 1]);
                }
                mundo->corpo[x][y - 1] = objeto->id;
                printf("id: %d Moveu para oeste\n", objeto->id);
                objeto->y = y - 1;
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
            mundo->corpo[objeto->x][objeto->y] = VAZIO;
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
            mundo->corpo[objeto->x][objeto->y] = VAZIO;
            objeto->proc++;
            return 0;
        }
    }

    return 0;
}