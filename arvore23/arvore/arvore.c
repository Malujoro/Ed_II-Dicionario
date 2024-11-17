#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

Arvore23 *no23_alocar()
{
    Arvore23 *no;
    no = (Arvore23 *)malloc(sizeof(Arvore23));

    if (!no)
    {
        printf("Erro ao alocar nó da árvore");
        exit(EXIT_FAILURE);
    }

    return no;
}

Arvore23 *no23_criar(Data info, Arvore23 *filho_esquerdo, Arvore23 *filho_centro)
{
    Arvore23 *no;
    no = no23_alocar();

    no->info1 = info;
    no->esquerdo = filho_esquerdo;
    no->centro = filho_centro;
    no->direito = NULL;
    no->n_infos = 1;
    return no;
}

// TODO na folha o Nó possui **
Arvore23 *no23_quebrar(Arvore23 *no, Data info, Data *promove, Arvore23 *filho_maior)
{
    Arvore23 *maior;
    if(info.numero > no->info2.numero)
    {
        *promove = no->info2;
        maior = no23_criar(info, no->direito, filho_maior);
    }
    else if (info.numero > no->info1.numero)
    {
        *promove = info;
        maior = no23_criar(no->info2, filho_maior, no->direito);
    }
    else
    {
        *promove = no->info1;
        maior = no23_criar(no->info2, no->centro, no->direito);
        no->info1 = info;
        no->centro = filho_maior;
    }
    no->n_infos = 1;

    return maior;
}

// TODO na folha há um retorno de nó
Arvore23 *no23_adicionar_info(Arvore23 *no, Data info, Arvore23 *filho_maior)
{
    if(info.numero > no->info1.numero)
    {
        no->info2 = info;
        no->direito = filho_maior;
    }
    else
    {
        no->info2 = no->info1;
        no->direito = no->centro;
        no->centro = filho_maior;
        no->info1 = info;
    }
    no->n_infos = 2;
}

Arvore23 *arvore23_criar()
{
    return NULL;
}

void arvore23_desalocar(Arvore23 **raiz);