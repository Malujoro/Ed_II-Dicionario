#ifndef LISTA_H
#define LISTA_H

typedef struct lista
{
    int info;
    struct lista *proximo;
} Lista;

Lista *no_lista_alocar();

Lista *no_lista_criar(int info);

Lista *lista_criar();

void lista_desalocar(Lista **lista);

void lista_inserir(Lista **lista, int info);

void lista_exibir(Lista *lista);

Lista *lista_buscar(Lista *lista, int info);

int lista_remover(Lista **lista, int info);

#endif