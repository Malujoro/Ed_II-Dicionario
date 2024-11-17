#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

int eh_folha(Arvore23 no)
{
    return no.esquerdo == NULL;
}

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
    else if(info.numero > no->info1.numero)
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
void no23_adicionar_info(Arvore23 *no, Data info, Arvore23 *filho_maior)
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

void arvore23_desalocar(Arvore23 **raiz)
{
    if(*raiz != NULL)
    {
        arvore23_desalocar(&((*raiz)->esquerdo));
        arvore23_desalocar(&((*raiz)->centro));

        if((*raiz)->n_infos == 2)
            arvore23_desalocar(&((*raiz)->direito));

        free(*raiz);
        *raiz = NULL;
    }
}

Arvore23 *arvore23_inserir(Arvore23 **raiz, Data info, Arvore23 *pai, Data *promove)
{
    Arvore23 *maior;
    maior = NULL;

    if(*raiz == NULL)
        *raiz = no23_criar(info, NULL, NULL);
    else
    {
        if(eh_folha(**raiz))
        {
            if((*raiz)->n_infos == 1)
                no23_adicionar_info(*raiz, info, NULL);
            else
            {
                maior = no23_quebrar(*raiz, info, promove, NULL);
                if(pai == NULL)
                {
                    *raiz = no23_criar(*promove, *raiz, maior);
                    maior = NULL;
                }
            }
        }
        else
        {
            if(info.numero < (*raiz)->info1.numero)
                maior = arvore23_inserir(&((*raiz)->esquerdo), info, *raiz, promove);
            else if((*raiz)->n_infos == 1 || info.numero < (*raiz)->info2.numero)
                maior = arvore23_inserir(&((*raiz)->centro), info, *raiz, promove);
            else
                maior = arvore23_inserir(&((*raiz)->direito), info, *raiz, promove);

            if(maior != NULL)
            {
                if((*raiz)->n_infos == 1)
                {
                    no23_adicionar_info(*raiz, *promove, maior);
                    maior = NULL;
                }
                else
                {
                    Data promove_aux;
                    maior = no23_quebrar(*raiz, *promove, &promove_aux, maior);
                    *promove = promove_aux;
                    if(pai == NULL)
                    {
                        *raiz = no23_criar(promove_aux, *raiz, maior);
                        maior = NULL;
                    }
                }
            }
        }

    }

    return maior;
}

void arvore23_exibir_pre(Arvore23 *raiz)
{
    if(raiz != NULL)
    {
        printf("[1º] %d -> ", raiz->info1.numero);
        if(raiz->n_infos == 2)
            printf("[2º] %d -> ", raiz->info2.numero);

        arvore23_exibir_pre(raiz->esquerdo);
        arvore23_exibir_pre(raiz->centro);
        if(raiz->n_infos == 2)
            arvore23_exibir_pre(raiz->direito);
    }
}

void arvore23_exibir_ordem(Arvore23 *raiz)
{
    if(raiz != NULL)
    {
        arvore23_exibir_ordem(raiz->esquerdo);
        printf("[1º] %d -> ", raiz->info1.numero);
        arvore23_exibir_ordem(raiz->centro);

        if(raiz->n_infos == 2)
        {
            printf("[2º] %d -> ", raiz->info2.numero);
            arvore23_exibir_ordem(raiz->direito);
        }
    }
}

void arvore23_exibir_pos(Arvore23 *raiz)
{
    if(raiz != NULL)
    {
        arvore23_exibir_pos(raiz->esquerdo);
        arvore23_exibir_pos(raiz->centro);
        if(raiz->n_infos == 2)
            arvore23_exibir_pos(raiz->direito);

        printf("[1º] %d -> ", raiz->info1.numero);
        if(raiz->n_infos == 2)
            printf("[2º] %d -> ", raiz->info2.numero);
    }
}


int main()
{
    int tam;
    tam = 10;
    int valores[] = {5000, 4000, 1000, 2000, 1500, 500, 300, 6000, 8000, 7000};

    // tam = 26;
    // int valores[] = {8000, 10000, 15000, 1000, 3000, 7000, 5800, 4200, 2500, 1800, 9000, 7500, 6500, 4300, 3500, 9500, 2100, 500, 900, 100, 600, 1700, 2400, 1250, 1750, 250};

    Arvore23 *arvore;
    arvore = arvore23_criar();
    Data *promove = (Data *) malloc(sizeof(Data));

    for(int i = 0; i < tam; i++)
    {
        Data info;
        info.numero = valores[i];
        
        arvore23_inserir(&arvore, info, NULL, promove);
        printf("\n\n%dª inserção [%d]: \n", i+1, info.numero);
        arvore23_exibir_pos(arvore);
    }
    printf("\n\n");
}