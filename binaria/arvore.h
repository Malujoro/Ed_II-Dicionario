#ifndef ARVOREBB_H
#define ARVOREBB_H

#include "../lista/lista.h"

typedef struct dataeng {
    char *palavraIngles;
    Lista *unidade;
} DataEng;

typedef struct arvorebb
{
    DataEng info;
    struct arvorebb *esquerdo;
    struct arvorebb *direito;
} ArvoreBB;

/*
    Função para alocar um nó da árvore
    Retorna o endereço do nó alocado
*/
ArvoreBB *nobb_alocar();

/*
    Função para criar o nó da árvore
    Tem como parâmetro a informação do nó 
    Retorna o endereço do nó criado
*/
ArvoreBB *nobb_criar(DataEng info);

/*
    Função para criar a árvore
    Retorna a "árvore vazia" (null)
*/
ArvoreBB *arvorebb_criar();

/*
    Função para desalocar a árvore
    Tem como parâmetro a referência da raiz da árvore
*/
void arvorebb_desalocar(ArvoreBB **raiz);

/*
    Função para adicionar uma informação na árvore (criando um nó)
    Tem como parâmetro a referência da raiz da árvore e a informação a ser adicionada
    Retorna o endereço do nó inserido (ou o nó existente, caso seja repetido)
*/
ArvoreBB *arvorebb_inserir(ArvoreBB **raiz, DataEng info);

/*
    Função para buscar um nó da árvore
    Tem como parâmetro a raiz da árvore e a palavra a ser buscada
    Caso encontre, retorna a palavra. Caso não, retorna Null
*/
ArvoreBB *arvorebb_buscar(ArvoreBB *raiz, char *palavra);

/*
    Função para exibir a árvore no formato "pré ordem"
    Tem como parâmetro o endereço da raiz da árvore
*/
void arvorebb_exibir_pre(ArvoreBB *raiz);

/*
    Função para exibir a árvore no formato "em ordem"
    Tem como parâmetro o endereço da raiz da árvore
*/
void arvorebb_exibir_ordem(ArvoreBB *raiz);

/*
    Função para exibir a árvore no formato "pós ordem"
    Tem como parâmetro o endereço da raiz da árvore
*/
void arvorebb_exibir_pos(ArvoreBB *raiz);

/*
    Função para remover um nó da árvore
    Tem como parâmetro a referência da raiz da árvore e a palavra a ser removida
    Retorna 1 caso seja removido (ou 0 caso não encontre)
*/
int arvorebb_remover(ArvoreBB **raiz, char *palavra);

#endif