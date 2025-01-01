#ifndef ARVORE23_H
#define ARVORE23_H

#include "../../binaria/arvore.h"

typedef struct datapt
{
    char *palavraPT;
    ArvoreBB *palavrasEng;
} DataPT;

typedef struct arvore23
{
    DataPT info1;
    DataPT info2;
    int n_infos;
    struct arvore23 *esquerdo;
    struct arvore23 *centro;
    struct arvore23 *direito;
} Arvore23;


Arvore23 *no23_alocar();

void no23_desalocar(Arvore23 **no);

Arvore23 *no23_criar(DataPT info, Arvore23 *filho_esquerdo, Arvore23 *filho_centro);

DataPT no23_maior_info(Arvore23 *raiz);


Arvore23 *arvore23_criar();

Arvore23 *arvore23_buscar(Arvore23 *raiz, char *info);

Arvore23 *arvore23_buscar_menor_filho(Arvore23 *raiz, Arvore23 **pai);

Arvore23 *arvore23_buscar_maior_filho(Arvore23 *raiz, Arvore23 **pai, DataPT *maior_valor);

Arvore23 *arvore23_buscar_pai(Arvore23 *raiz, char *info);

Arvore23 *arvore23_buscar_maior_pai(Arvore23 *raiz, char *info);

Arvore23 *arvore23_buscar_menor_pai(Arvore23 *raiz, char *info);

void arvore23_desalocar(Arvore23 **raiz);

Arvore23 *arvore23_inserir(Arvore23 **raiz, DataPT info, Arvore23 *pai, DataPT *promove);

int arvore23_remover1(Arvore23 **raiz, char *info, Arvore23 *pai, Arvore23 **origem, Arvore23 **maior);

int arvore23_remover2(Arvore23 **raiz, char *info, Arvore23 *pai, Arvore23 **origem, Arvore23 **maior);

int arvore23_remover(Arvore23 **raiz, char *info);

int arvore23_rebalancear(Arvore23 **raiz, char *info, Arvore23 **maior);

void arvore23_exibir_pre(Arvore23 *raiz);

void arvore23_exibir_ordem(Arvore23 *raiz);

void arvore23_exibir_pos(Arvore23 *raiz);

#endif