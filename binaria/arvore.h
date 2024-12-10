#ifndef ARVOREBB_H
#define ARVOREBB_H

typedef struct dataeng {
    char *palavraIngles;
    int unidade;
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
ArvoreBB *no_alocar();

/*
    Função para criar o nó da árvore
    Tem como parâmetro a informação do nó 
    Retorna o endereço do nó criado
*/
ArvoreBB *no_criar(DataEng info);

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
    Retorna 1 caso a adição tenha sido feita com sucesso, e 0 caso o código já exista na árvore
*/
int arvorebb_inserir(ArvoreBB **raiz, DataEng info);

/*
    Função para buscar um nó da árvore
    Tem como parâmetro o código a ser buscado
    Caso encontre, retorna o código. Caso não, retorna Null
*/
ArvoreBB *arvorebb_buscar(ArvoreBB *raiz, char *palavra);

/*
    Função para exibir a árvore no formato "em ordem"
    Tem como parâmetro o endereço da raiz da árvore
*/
void arvorebb_exibir_pre(ArvoreBB *raiz);
void arvorebb_exibir_pos(ArvoreBB *raiz);
void arvorebb_exibir_ordem(ArvoreBB *raiz);

/*
    Função para remover um nó da árvore
    Tem como parâmetro a referência da raiz da árvore e o código a ser removida
    Retorna o nó removido (ou null caso não encontre)
*/
int arvorebb_remover(ArvoreBB **raiz, char *palavra);

#endif