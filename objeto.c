#include "objeto.h"

// *int verificaAdjacentes(Mundo* mundo, Objeto objeto){
//     int pos[]
// }


void instanciaElementos(int linha, int coluna, Elementos *elementos)
{
    elementos->numeroDeObjetos = 0;
    elementos->objetos = (Objeto *) malloc(linha * coluna * sizeof(Objeto));
    for (int i = 0; i < linha * coluna; i++)
    {
        elementos->objetos[i].id = -1;
    }
}
