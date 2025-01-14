#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"

ArvoreVP *novp_alocar()
{
    ArvoreVP *no;
    no = (ArvoreVP *)malloc(sizeof(ArvoreVP));

    if (!no)
    {
        printf("Erro ao alocar nó da árvore");
        exit(EXIT_FAILURE);
    }

    return no;
}

ArvoreVP *novp_criar(DataPT info)
{
    ArvoreVP *no;
    no = novp_alocar();

    no->info = info;
    no->cor = VERMELHO;
    no->esquerdo = NULL;
    no->direito = NULL;
    return no;
}

ArvoreVP *arvorevp_criar()
{
    return NULL;
}

void novp_desalocar(ArvoreVP **raiz)
{
    if(*raiz != NULL)
    {
        free((*raiz)->info.palavraPT);
        (*raiz)->info.palavraPT = NULL;

        arvorebb_desalocar(&((*raiz)->info.palavrasEng));

        free(*raiz);
        *raiz = NULL;
    }

}

void arvorevp_desalocar(ArvoreVP **raiz)
{
    if ((*raiz) != NULL)
    {
        if ((*raiz)->esquerdo != NULL)
            arvorevp_desalocar(&((*raiz)->esquerdo));

        if ((*raiz)->direito != NULL)
            arvorevp_desalocar(&((*raiz)->direito));

        novp_desalocar(raiz);
    }
}

int retornar_cor(ArvoreVP *raiz)
{
    return raiz == NULL ? PRETO : raiz->cor;
}

void rotacao_esquerda(ArvoreVP **raiz)
{
    ArvoreVP *aux;

    aux = (*raiz)->direito;
    (*raiz)->direito = aux->esquerdo;
    aux->esquerdo = (*raiz);
    (*raiz) = aux;

    (*raiz)->cor = (*raiz)->esquerdo->cor;
    (*raiz)->esquerdo->cor = VERMELHO;
    
}

void rotacao_direita(ArvoreVP **raiz)
{
    ArvoreVP *aux;

    aux = (*raiz)->esquerdo;
    (*raiz)->esquerdo = aux->direito;
    aux->direito = (*raiz);
    (*raiz) = aux;

    (*raiz)->cor = (*raiz)->direito->cor;
    (*raiz)->direito->cor = VERMELHO;

}

void trocar_cor(ArvoreVP *raiz)
{
    raiz->cor = !(raiz->cor);

    if(raiz->esquerdo != NULL)
        raiz->esquerdo->cor = !(raiz->esquerdo->cor);

    if(raiz->direito != NULL)
        raiz->direito->cor =!(raiz->direito->cor);
}

void balancear_vp(ArvoreVP **raiz)
{
    if(*raiz != NULL)
    {
        if(retornar_cor((*raiz)->esquerdo) == PRETO && retornar_cor((*raiz)->direito) == VERMELHO)
            rotacao_esquerda(raiz);

        if(retornar_cor((*raiz)->esquerdo) == VERMELHO && retornar_cor((*raiz)->esquerdo->esquerdo) == VERMELHO)
            rotacao_direita(raiz);

        if(retornar_cor((*raiz)->esquerdo) == VERMELHO && retornar_cor((*raiz)->direito) == VERMELHO)
            trocar_cor(*raiz);
    }
}

ArvoreVP *arvorevp_inserir_no(ArvoreVP **raiz, DataPT info)
{
    ArvoreVP *inseriu;
    inseriu = NULL;

    if ((*raiz) == NULL)
    {
        (*raiz) = novp_criar(info);
        inseriu = *raiz;
    }
    else
    {
        int resultado = strcmp(info.palavraPT, (*raiz)->info.palavraPT);

        if (resultado < 0)
            inseriu = arvorevp_inserir_no(&((*raiz)->esquerdo), info);
        else if (resultado > 0)
            inseriu = arvorevp_inserir_no(&((*raiz)->direito), info);
        else
            inseriu = *raiz;

        balancear_vp(raiz);
    }

    return inseriu;
}

ArvoreVP *arvorevp_buscar(ArvoreVP *raiz, char *palavra)
{
    ArvoreVP *retorno;

    if(raiz != NULL)
    {
        int resultado = strcmp(palavra, raiz->info.palavraPT);

        if(resultado == 0)
            retorno = raiz;
        else if(resultado < 0)
            retorno = arvorevp_buscar(raiz->esquerdo, palavra);
        else if(resultado > 0)
            retorno = arvorevp_buscar(raiz->direito, palavra);
    }
    else
        retorno = NULL;
    
    return retorno;
}

void exibir_dataPT(DataPT info)
{
    printf("Palavra[PT]: %s\n", info.palavraPT);
    arvorebb_exibir_pre(info.palavrasEng);
    printf("\n");
}

void arvorevp_exibir_pre(ArvoreVP *raiz)
{
    if (raiz != NULL)
    {
        printf("Cor: [%s] \n", retornar_cor(raiz) == PRETO ? "Preto" : "Vermelho");
        exibir_dataPT(raiz->info);
        arvorevp_exibir_pre(raiz->esquerdo);
        arvorevp_exibir_pre(raiz->direito);
    }
}

void arvorevp_exibir_ordem(ArvoreVP *raiz)
{
    if (raiz != NULL)
    {
        arvorevp_exibir_ordem(raiz->esquerdo);
        printf("Cor: [%s] \n", retornar_cor(raiz) == PRETO ? "Preto" : "Vermelho");
        exibir_dataPT(raiz->info);
        arvorevp_exibir_ordem(raiz->direito);
    }
}

void arvorevp_exibir_pos(ArvoreVP *raiz)
{
    if (raiz != NULL)
    {
        arvorevp_exibir_pos(raiz->esquerdo);
        arvorevp_exibir_pos(raiz->direito);
        printf("Cor: [%s] \n", retornar_cor(raiz) == PRETO ? "Preto" : "Vermelho");
        exibir_dataPT(raiz->info);
    }
}

void mover_esquerda(ArvoreVP **raiz)
{
    trocar_cor(*raiz);

    if((*raiz)->direito != NULL && retornar_cor((*raiz)->direito->esquerdo) == VERMELHO)
    {
        rotacao_direita(&((*raiz)->direito));
        rotacao_esquerda(raiz);
        trocar_cor(*raiz);
    }
}

void mover_direita(ArvoreVP **raiz)
{
    trocar_cor(*raiz);

    if((*raiz)->esquerdo != NULL && retornar_cor((*raiz)->esquerdo->esquerdo) == VERMELHO)
    {
        rotacao_direita(raiz);
        trocar_cor(*raiz);
    }
}

void remover_menor(ArvoreVP **raiz)
{
    if((*raiz)->esquerdo == NULL)
        novp_desalocar(raiz);
    else
    {
        if(retornar_cor((*raiz)->esquerdo) == PRETO && retornar_cor((*raiz)->esquerdo->esquerdo) == PRETO)
            mover_esquerda(raiz);

        remover_menor(&((*raiz)->esquerdo));
        balancear_vp(raiz);
    }
        
}

static ArvoreVP *menor_filho(ArvoreVP *raiz)
{
    ArvoreVP *aux;
    aux = raiz;

    while (aux->esquerdo != NULL)
        aux = aux->esquerdo;

    return aux;
}

int arvorevp_remover_no(ArvoreVP **raiz, char *palavra)
{
    int removeu = 1;

    if((*raiz) != NULL)
    {
        int resultado = strcmp(palavra, (*raiz)->info.palavraPT);

        if(resultado < 0)
        {
            if((*raiz)->esquerdo != NULL)
            {
                if(retornar_cor((*raiz)->esquerdo) == PRETO && retornar_cor((*raiz)->esquerdo->esquerdo) == PRETO)
                    mover_esquerda(raiz);
            }
                
            removeu = arvorevp_remover_no(&((*raiz)->esquerdo), palavra);
        }
        else {
            if(retornar_cor((*raiz)->esquerdo) == VERMELHO)
                rotacao_direita(raiz);

            if(resultado == 0 && (*raiz)->direito == NULL)
                novp_desalocar(raiz);
            else
            {
                if((*raiz)->direito != NULL)
                {
                    if(retornar_cor((*raiz)->direito) == PRETO && retornar_cor((*raiz)->direito->esquerdo) == PRETO)
                        mover_direita(raiz);
                }

                if(strcmp(palavra, (*raiz)->info.palavraPT) == 0)
                {
                    ArvoreVP *menor;
                    menor = menor_filho((*raiz)->direito);
                    (*raiz)->info = menor->info;
                    remover_menor(&((*raiz)->direito));
                    printf("\n");
                }
                else
                    removeu = arvorevp_remover_no(&((*raiz)->direito), palavra);
            }
        }
        balancear_vp(raiz);
    }
    else
        removeu = 0;

    return removeu;
}

ArvoreVP *arvorevp_inserir(ArvoreVP **raiz, DataPT info)
{
    ArvoreVP *inseriu;
    inseriu = arvorevp_inserir_no(raiz, info);

    if(*raiz != NULL)
        (*raiz)->cor = PRETO;

    return inseriu;
}

int arvorevp_remover(ArvoreVP **raiz, char *palavra)
{
    int removeu = arvorevp_remover_no(raiz, palavra);
    
    if(*raiz != NULL)
        (*raiz)->cor = PRETO;

    return removeu;
}