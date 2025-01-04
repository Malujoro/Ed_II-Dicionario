#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

Lista *no_lista_alocar()
{
    Lista *no;
    no = (Lista *) malloc(sizeof(Lista));
    
    if(!no)
    {
        printf("Erro ao alocar nó da lista");
        exit(EXIT_FAILURE);
    }

    return no;
}

Lista *no_lista_criar(int info)
{
    Lista *no;
    no = no_lista_alocar();
    no->info = info;
    no->proximo = NULL;
    return no;
}

Lista *lista_criar()
{
    return NULL;
}

void lista_desalocar(Lista **lista)
{
    if(*lista != NULL)
    {
        if((*lista)->proximo != NULL)
            lista_desalocar(&(*lista)->proximo);

        free(*lista);
        *lista = NULL;
    }
}

void lista_inserir(Lista **lista, int info)
{
    Lista *no;
    no = no_lista_criar(info);

    no->proximo = (*lista);
    (*lista) = no;
}

void lista_exibir(Lista *lista)
{
    Lista *no;
    no = lista;
    while(no != NULL)
    {
        printf("%d -> ", no->info);
        no = no->proximo;
    }
}
 
Lista *lista_buscar(Lista *lista, int info)
{
    Lista *aux;
    aux = lista;
    
    while(aux != NULL && aux->info != info)
        aux = aux->proximo;

    return aux;
}

int lista_remover(Lista **lista, int info)
{
    int removeu = 1;

    if(*lista != NULL)
    {
        if((*lista)->info == info)
        {
            Lista *aux;
            aux = (*lista)->proximo;
            free(*lista);
            *lista = aux;
        }
        else
            removeu = lista_remover(&((*lista)->proximo), info);
    }
    else
        removeu = 0;
    
    return removeu;
}