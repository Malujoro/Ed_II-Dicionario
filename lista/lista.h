#ifndef LISTA_H
#define LISTA_H

typedef struct lista
{
    int info;
    struct lista *proximo;
} Lista;

Lista *no_lista_aloca();

Lista *no_lista_cria(int info);

Lista *lista_cria();

void lista_desaloca(Lista **lista);

void lista_inserir(Lista **lista, int info);

void lista_exibir(Lista *lista);

Lista *lista_buscar(Lista *lista, int info);

#endif