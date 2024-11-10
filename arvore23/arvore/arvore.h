#ifndef ARVORE23_H
#define ARVORE23_H
typedef struct data
{
    int numero;
} Data;

typedef struct arvore23
{
    Data info1;
    Data info2;
    int n_infos;
    struct arvore23 *esquerdo;
    struct arvore23 *centro;
    struct arvore23 *direito;
} Arvore23;


Arvore23 *no23_alocar();

Arvore23 *no23_criar(Data info);

Arvore23 *arvore23_criar();

void arvore23_desaloca(Arvore23 **raiz);

int arvore23_inserir();


int arvore23_remover();

Arvore23 *arvore23_buscar();

void arvore23_exibir();

#endif