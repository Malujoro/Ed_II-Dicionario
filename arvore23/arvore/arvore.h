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


int arvore23_remover1(Arvore23 **raiz, int info, Arvore23 *pai, Arvore23 **origem, Arvore23 **maior);
int arvore23_remover2(Arvore23 **raiz, int info, Arvore23 *pai, Arvore23 **origem, Arvore23 **maior);
int arvore23_rebalancear(Arvore23 **raiz, int info, Arvore23 **maior);

Arvore23 *arvore23_buscar();

void arvore23_exibir();

void no23_adicionar_info(Arvore23 *no, Data info, Arvore23 *filho_maior);
Arvore23 *buscar_menor_filho(Arvore23 *raiz, Arvore23 **pai);
int movimento_onda(Data saindo, Data *entrada, Arvore23 *pai, Arvore23 **origem, Arvore23 **raiz, Arvore23 **maior);
int movimento_onda_esq2dir(Data saindo, Data *entrada, Arvore23 *pai, Arvore23 **origem, Arvore23 **raiz, Arvore23 **maior);


#endif