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

/*
    Função para verificar se uma palavra está na info1
    Retorna 1 caso sim, e 0 caso não
*/
int eh_info1(Arvore23 no, char *info);

/*
    Função para verificar se uma palavra está na info2
    Retorna 1 caso sim, e 0 caso não
*/
int eh_info2(Arvore23 no, char *info);

/*
    Função para alocar um nó da árvore
    Retorna o endereço do nó alocado
*/
Arvore23 *no23_alocar();

/*
    Função para desalocar o nó
    Tem como parâmetro a referência do nó
*/
void no23_desalocar(Arvore23 **no);

/*
    Função para criar o nó da árvore
    Tem como parâmetro a informação do nó e o ponteiro para os filhos esquerdo e direito
    Retorna o endereço do nó criado
*/
Arvore23 *no23_criar(DataPT info, Arvore23 *filho_esquerdo, Arvore23 *filho_centro);

/*
    Função para retornar a maior info de um nó
    Tem como parâmetro o ponteiro para a raiz da árvore
    Retorna uma cópia da maior info
*/
DataPT no23_maior_info(Arvore23 *raiz);

/*
    Função para criar a árvore
    Retorna a "árvore vazia" (null)
*/
Arvore23 *arvore23_criar();

/*
    Função para buscar um nó da árvore
    Tem como parâmetro a raiz da árvore e a palavra a ser buscada
    Caso encontre, retorna a palavra. Caso não, retorna Null
*/
Arvore23 *arvore23_buscar(Arvore23 *raiz, char *info);

/*
    Função para buscar o menor filho de um nó
    Tem como parâmetro a raiz da árvore e a referência para retornar o pai do nó a ser buscado
    Caso encontre, retorna o nó. Caso não, retorna Null
*/
Arvore23 *arvore23_buscar_menor_filho(Arvore23 *raiz, Arvore23 **pai);

/*
    Função para buscar o maior filho de um nó
    Tem como parâmetro a raiz da árvore, a referência para retornar o pai do nó a ser buscado e a referência do maior valor desse nó encontrado
    Caso encontre, retorna o nó. Caso não, retorna Null
*/
Arvore23 *arvore23_buscar_maior_filho(Arvore23 *raiz, Arvore23 **pai, DataPT *maior_valor);

/*
    Função para buscar o pai de um nó
    Tem como parâmetro a raiz da árvore e a info que o pai deve ser buscado
    Caso encontre, retorna o pai. Caso não, retorna Null
*/
Arvore23 *arvore23_buscar_pai(Arvore23 *raiz, char *info);

/*
    Função para buscar o "primeiro maior pai" de um nó
    Tem como parâmetro a raiz da árvore e a info que o pai deve ser buscado
    Caso encontre, retorna o pai. Caso não, retorna Null
*/
Arvore23 *arvore23_buscar_maior_pai(Arvore23 *raiz, char *info);

/*
    Função para buscar o "primeiro menor pai" de um nó
    Tem como parâmetro a raiz da árvore e a info que o pai deve ser buscado
    Caso encontre, retorna o pai. Caso não, retorna Null
*/
Arvore23 *arvore23_buscar_menor_pai(Arvore23 *raiz, char *info);

/*
    Função para desalocar a árvore
    Tem como parâmetro a referência da raiz da árvore
*/
void arvore23_desalocar(Arvore23 **raiz);

/*
    Função para adicionar uma informação na árvore (criando um nó)
    Tem como parâmetro a referência da raiz da árvore e a informação a ser adicionada
    Retorna o endereço do nó inserido (ou o nó existente, caso seja repetido)
*/
Arvore23 *arvore23_inserir(Arvore23 **raiz, DataPT info);

/*
    Função para remover um nó da árvore seguindo uma lógica de onda que "busca os valores da direita"
    Tem como parâmetro a referência da raiz da árvore, a palavra a ser removida, o ponteiro para o pai daquele nó, a referência da raiz "global" (origem) da árvore e a referência do "maior nó" (Caso seja necessário rebalancear, o indicador do local)
    Retorna 1 caso seja removido, 0 caso não encontre ou -1 caso seja necessário rebalancear
*/
int arvore23_remover1(Arvore23 **raiz, char *info, Arvore23 *pai, Arvore23 **origem, Arvore23 **maior);

/*
    Função para remover um nó da árvore seguindo uma lógica de onda que "busca os valores da esquerda"
    Tem como parâmetro a referência da raiz da árvore, a palavra a ser removida, o ponteiro para o pai daquele nó, a referência da raiz "global" (origem) da árvore e a referência do "maior nó" (Caso seja necessário rebalancear, o indicador do local)
    Retorna 1 caso seja removido, 0 caso não encontre ou -1 caso seja necessário rebalancear
*/
int arvore23_remover2(Arvore23 **raiz, char *info, Arvore23 *pai, Arvore23 **origem, Arvore23 **maior);

/*
    Função para remover um nó da árvore
    Tem como parâmetro a referência da raiz da árvore e a palavra a ser removida
    Retorna 1 caso seja removido (ou 0 caso não encontre)
*/
int arvore23_remover(Arvore23 **raiz, char *info);

/*
    Função para "rebalancear" uma árvore 2-3 (quando a remoção pede por uma junção de nó)
    Tem como parâmetro a referência da raiz da árvore, a palavra (local) que precisa de rebalanceamento e a referência do "maior nó" (será utilizado quando voltar da recursão)
    Retorna 1 caso seja rebalanceado (ou -1 caso não seja possível rebalancear)
*/
int arvore23_rebalancear(Arvore23 **raiz, char *info, Arvore23 **maior);

/*
    Função para exibir a árvore no formato "pré ordem"
    Tem como parâmetro o endereço da raiz da árvore
*/
void arvore23_exibir_pre(Arvore23 *raiz);

/*
    Função para exibir a árvore no formato "em ordem"
    Tem como parâmetro o endereço da raiz da árvore
*/
void arvore23_exibir_ordem(Arvore23 *raiz);

/*
    Função para exibir a árvore no formato "pós ordem"
    Tem como parâmetro o endereço da raiz da árvore
*/
void arvore23_exibir_pos(Arvore23 *raiz);

#endif