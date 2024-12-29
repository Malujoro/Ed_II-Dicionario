#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

Lista *no_lista_aloca()
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

Lista *no_lista_cria(int info)
{
    Lista *no;
    no = no_lista_aloca();
    no->info = info;
    no->proximo = NULL;
    return no;
}

Lista *lista_cria()
{
    return NULL;
}

void lista_desaloca(Lista **lista)
{
    if(*lista != NULL)
    {
        if((*lista)->proximo != NULL)
            lista_desaloca(&(*lista)->proximo);

        free(*lista);
        *lista = NULL;
    }
}

void lista_inserir(Lista **lista, int info)
{
    Lista *no;
    no = no_lista_cria(info);

    no->proximo = (*lista);
    (*lista) = no;
}

void lista_exibir(Lista *lista)
{
    printf("Lista\n");

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
    Lista *aux = lista;
    
    while(aux != NULL && aux->info != info)
        aux = aux->proximo;

    return aux;
}