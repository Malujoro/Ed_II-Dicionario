#ifndef ARVOREVP_H
#define ARVOREVP_H

#include "../../binaria/arvore.h"

#define VERMELHO 0
#define PRETO 1

typedef struct datapt {
    char *palavraPT;
    ArvoreBB *palavrasEng;
} DataPT;

typedef struct arvorevp
{
    DataPT info;
    int cor;
    struct arvorevp *esquerdo;
    struct arvorevp *direito;
} ArvoreVP;

/*
    Função para alocar um nó da árvore
    Retorna o endereço do nó alocado
*/
ArvoreVP *novp_alocar();

/*
    Função para criar o nó da árvore
    Tem como parâmetro a informação do nó 
    Retorna o endereço do nó criado
*/
ArvoreVP *novp_criar(DataPT info);

/*
    Função para criar a árvore
    Retorna a "árvore vazia" (null)
*/
ArvoreVP *arvorevp_criar();

/*
    Função para desalocar a árvore
    Tem como parâmetro a referência da raiz da árvore
*/
void arvorevp_desalocar(ArvoreVP **raiz);

/*
    Função para adicionar uma informação na árvore (criando um nó)
    Tem como parâmetro a referência da raiz da árvore e a informação a ser adicionada
    Retorna o endereço do nó inserido (ou o nó existente, caso seja repetido)
*/
ArvoreVP *arvorevp_inserir(ArvoreVP **raiz, DataPT info);

/*
    Função para buscar um nó da árvore
    Tem como parâmetro o endereço da raiz da árvore e a palavra a ser buscada
    Caso encontre, retorna a palavra. Caso não, retorna Null
*/
ArvoreVP *arvorevp_buscar(ArvoreVP *raiz, char *palavra);

/*
    Função para exibir a árvore no formato "pré ordem"
    Tem como parâmetro o endereço da raiz da árvore
*/
void arvorevp_exibir_pre(ArvoreVP *raiz);

/*
    Função para exibir a árvore no formato "em ordem"
    Tem como parâmetro o endereço da raiz da árvore
*/
void arvorevp_exibir_ordem(ArvoreVP *raiz);

/*
    Função para exibir a árvore no formato "pós ordem"
    Tem como parâmetro o endereço da raiz da árvore
*/
void arvorevp_exibir_pos(ArvoreVP *raiz);

/*
    Função para remover um nó da árvore
    Tem como parâmetro a referência da raiz da árvore e a palavra a ser removida
    Retorna 1 caso seja removido (ou 0 caso não encontre)
*/
int arvorevp_remover(ArvoreVP **raiz, char *palavra);

#endif