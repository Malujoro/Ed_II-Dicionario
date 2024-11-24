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

Arvore23 *no23_criar(Data info, Arvore23 *filho_esquerdo, Arvore23 *filho_centro);

Arvore23 *arvore23_criar();

void arvore23_desalocar(Arvore23 **raiz);

Arvore23 *arvore23_inserir(Arvore23 **raiz, Data info, Arvore23 *pai, Data *promove);


int arvore23_remover();

Arvore23 *arvore23_buscar();

void arvore23_exibir();

Arvore23 *buscar_menor_filho(Arvore23 *raiz, Arvore23 **pai, Data *menor_info);
void movimento_onda(Data saindo, Data *entrada, Arvore23 *pai, Arvore23 **origem, Arvore23 **raiz);


#endif