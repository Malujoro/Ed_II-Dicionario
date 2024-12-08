#ifndef ARVOREVP_H
#define ARVOREVP_H

#define VERMELHO 0
#define PRETO 1

typedef struct data {
    int numero;
} Data;

typedef struct arvorevp
{
    Data info;
    int cor;
    struct arvorevp *esquerdo;
    struct arvorevp *direito;
} ArvoreVP;

/*
    Função para alocar um nó da árvore
    Retorna o endereço do nó alocado
*/
ArvoreVP *no_alocar();

/*
    Função para criar o nó da árvore
    Tem como parâmetro a informação do nó 
    Retorna o endereço do nó criado
*/
ArvoreVP *no_criar(Data info);

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
    Retorna 1 caso a adição tenha sido feita com sucesso, e 0 caso o código já exista na árvore
*/
int arvorevp_inserir(ArvoreVP **raiz, Data info);

/*
    Função para buscar um nó da árvore
    Tem como parâmetro o código a ser buscado
    Caso encontre, retorna o código. Caso não, retorna Null
*/
ArvoreVP *arvorevp_buscar(ArvoreVP *raiz, int codigo);

/*
    Função para exibir a árvore no formato "em ordem"
    Tem como parâmetro o endereço da raiz da árvore
*/
void arvorevp_exibir_pre(ArvoreVP *raiz);
void arvorevp_exibir_ordem(ArvoreVP *raiz);
void arvorevp_exibir_ordem(ArvoreVP *raiz);

/*
    Função para remover um nó da árvore
    Tem como parâmetro a referência da raiz da árvore e o código a ser removida
    Retorna o nó removido (ou null caso não encontre)
*/
int arvorevp_remover(ArvoreVP **raiz, int codigo);

#endif