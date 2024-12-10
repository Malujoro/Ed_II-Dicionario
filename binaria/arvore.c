#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"

ArvoreBB *no_alocar()
{
    ArvoreBB *no;
    no = (ArvoreBB *)malloc(sizeof(ArvoreBB));

    if (!no)
    {
        printf("Erro ao alocar nó da árvore de matrículas");
        exit(EXIT_FAILURE);
    }

    return no;
}

ArvoreBB *no_criar(DataEng info)
{
    ArvoreBB *no;
    no = no_alocar();

    no->info = info;
    no->esquerdo = NULL;
    no->direito = NULL;
    return no;
}

ArvoreBB *arvorebb_criar()
{
    return NULL;
}

void arvorebb_desalocar(ArvoreBB **raiz)
{
    if ((*raiz) != NULL)
    {
        if ((*raiz)->esquerdo != NULL)
            arvorebb_desalocar(&((*raiz)->esquerdo));

        if ((*raiz)->direito != NULL)
            arvorebb_desalocar(&((*raiz)->direito));

        free(*raiz);
        *raiz = NULL;
    }
}

int arvorebb_inserir(ArvoreBB **raiz, DataEng info)
{
    int inseriu = 1;
    if ((*raiz) == NULL)
        (*raiz) = no_criar(info);
    else
    {
        int resultado = strcmp(info.palavraIngles, (*raiz)->info.palavraIngles);

        if (resultado < 0)
            inseriu = arvorebb_inserir(&((*raiz)->esquerdo), info);
        else if (resultado > 0)
            inseriu = arvorebb_inserir(&((*raiz)->direito), info);
        else
            inseriu = 0;
    }

    return inseriu;
}

ArvoreBB *arvorebb_buscar(ArvoreBB *raiz, char *palavra)
{
    ArvoreBB *retorno;

    if(raiz != NULL)
    {
        int resultado = strcmp(palavra, raiz->info.palavraIngles);

        if(resultado == 0)
            retorno = raiz;
        else if(resultado < 0)
            retorno = arvorebb_buscar(raiz->esquerdo, palavra);
        else if(resultado > 0)
            retorno = arvorebb_buscar(raiz->direito, palavra);
    }
    else
        retorno = NULL;
    
    return retorno;
}

void arvorebb_exibir_pre(ArvoreBB *raiz)
{
    if (raiz != NULL)
    {
        printf("[%d] %s | ", raiz->info.unidade, raiz->info.palavraIngles);
        arvorebb_exibir_pre(raiz->esquerdo);
        arvorebb_exibir_pre(raiz->direito);
    }
}


static int no_folha(ArvoreBB *raiz)
{
    return raiz->esquerdo == NULL && raiz->direito == NULL;
}

static ArvoreBB *endereco_filho_unico(ArvoreBB *raiz)
{
    ArvoreBB *filho;

    if (raiz->esquerdo == NULL)
        filho = raiz->direito;
    else if (raiz->direito == NULL)
        filho = raiz->esquerdo;
    else
        filho = NULL;

    return filho;
}

static ArvoreBB *menor_filho(ArvoreBB *raiz)
{
    ArvoreBB *aux;
    aux = raiz;

    while (aux->esquerdo != NULL)
        aux = aux->esquerdo;

    return aux;
}

int arvorebb_remover(ArvoreBB **raiz, char *palavra)
{
    ArvoreBB *aux, *endereco_filho, *endereco_menor_filho;
    int removeu = 1;

    if ((*raiz) != NULL)
    {
        int resultado = strcmp(palavra, (*raiz)->info.palavraIngles);

        if (resultado == 0)
        {
            if (no_folha(*raiz))
            {
                aux = *raiz;
                *raiz = NULL;
                free(aux);
            }
            else if ((endereco_filho = endereco_filho_unico(*raiz)) != NULL)
            {
                aux = *raiz;
                *raiz = endereco_filho;
                free(aux);
            }
            else
            {
                endereco_menor_filho = menor_filho((*raiz)->direito);
                (*raiz)->info = endereco_menor_filho->info;
                arvorebb_remover(&((*raiz)->direito), endereco_menor_filho->info.palavraIngles);
            }
        }
        else if (resultado < 0)
            removeu = arvorebb_remover(&((*raiz)->esquerdo), palavra);
        else if (resultado > 0)
            removeu = arvorebb_remover(&((*raiz)->direito), palavra);
    }
    else
        removeu = 0;

    return removeu;
}

// int main()
// {
//     int tam, tam_remov;

//     char *valores[] = {"banana", "ajuda", "abacate", "maca", "melao", "arroz", "maao"};
//     char *removidos[] = {"arroz", "ajuda"};

//     tam = sizeof(valores) / sizeof(char *);
//     tam_remov = sizeof(removidos) / sizeof(char *);

//     ArvoreBB *arvore;
//     arvore = arvorebb_criar();

//     for(int i = 0; i < tam; i++)
//     {
//         DataEng info;
//         info.palavraIngles = valores[i];
//         info.unidade = i % 2;

//         arvorebb_inserir(&arvore, info);
//     }
//     printf("\n\nÁrvore após inserção: \n");
//     arvorebb_exibir_pre(arvore);

//     for(int i = 0; i < tam_remov; i++)
//     {
//         arvorebb_remover(&arvore, removidos[i]);
//         printf("\n\nÁrvore após remover %s:\n", removidos[i]);
//         arvorebb_exibir_pre(arvore);
//     }

//     printf("\n\n");
//     return 0;
// }