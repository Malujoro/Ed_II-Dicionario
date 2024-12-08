#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

ArvoreVP *no_alocar()
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

ArvoreVP *no_criar(Data info)
{
    ArvoreVP *no;
    no = no_alocar();

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

void no_desalocar(ArvoreVP **raiz)
{
    free(*raiz);
    *raiz = NULL;
}

void arvorevp_desalocar(ArvoreVP **raiz)
{
    if ((*raiz) != NULL)
    {
        if ((*raiz)->esquerdo != NULL)
            arvorevp_desalocar(&((*raiz)->esquerdo));

        if ((*raiz)->direito != NULL)
            arvorevp_desalocar(&((*raiz)->direito));

        no_desalocar(raiz);
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

int arvorevp_inserir_no(ArvoreVP **raiz, Data info)
{
    int inseriu = 1;

    if ((*raiz) == NULL)
        (*raiz) = no_criar(info);
    else
    {
        if (info.numero < (*raiz)->info.numero)
            inseriu = arvorevp_inserir_no(&((*raiz)->esquerdo), info);
        else if (info.numero > (*raiz)->info.numero)
            inseriu = arvorevp_inserir_no(&((*raiz)->direito), info);
        else
            inseriu = 0;

        balancear_vp(raiz);
    }

    return inseriu;
}

ArvoreVP *arvorevp_buscar(ArvoreVP *raiz, int valor)
{
    ArvoreVP *retorno;

    if(raiz != NULL)
    {
        if(valor == raiz->info.numero)
            retorno = raiz;
        else if(valor < raiz->info.numero)
            retorno = arvorevp_buscar(raiz->esquerdo, valor);
        else if(valor > raiz->info.numero)
            retorno = arvorevp_buscar(raiz->direito, valor);
    }
    else
        retorno = NULL;
    
    return retorno;
}

void arvorevp_exibir_pre(ArvoreVP *raiz)
{
    if (raiz != NULL)
    {
        printf("%d | Cor: [%s] \n", raiz->info.numero, retornar_cor(raiz) == PRETO ? "Preto" : "Vermelho");
        arvorevp_exibir_pre(raiz->esquerdo);
        arvorevp_exibir_pre(raiz->direito);
    }
}

void arvorevp_exibir_ordem(ArvoreVP *raiz)
{
    if (raiz != NULL)
    {
        arvorevp_exibir_ordem(raiz->esquerdo);
        printf("%d | Cor: [%s] \n", raiz->info.numero, retornar_cor(raiz) == PRETO ? "Preto" : "Vermelho");
        arvorevp_exibir_ordem(raiz->direito);
    }
}

void arvorevp_exibir_pos(ArvoreVP *raiz)
{
    if (raiz != NULL)
    {
        arvorevp_exibir_pos(raiz->esquerdo);
        arvorevp_exibir_pos(raiz->direito);
        printf("%d | Cor: [%s] \n", raiz->info.numero, retornar_cor(raiz) == PRETO ? "Preto" : "Vermelho");
    }
}

// TODO chance de segment fault por não verificar raiz->direito != NULL
void mover_esquerda(ArvoreVP **raiz)
{
    trocar_cor(*raiz);

    if(retornar_cor((*raiz)->direito->esquerdo) == VERMELHO)
    {
        rotacao_direita(&((*raiz)->direito));
        rotacao_esquerda(raiz);
        trocar_cor(*raiz);
    }
}

void mover_direita(ArvoreVP **raiz)
{
    trocar_cor(*raiz);

    if(retornar_cor((*raiz)->esquerdo->esquerdo) == VERMELHO)
    {
        rotacao_direita(raiz);
        trocar_cor(*raiz);
    }
}

void remover_menor(ArvoreVP **raiz)
{
    if((*raiz)->esquerdo == NULL)
        no_desalocar(raiz);
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

int arvorevp_remover_no(ArvoreVP **raiz, int valor)
{
    int removeu = 1;

    if((*raiz) != NULL)
    {
        if(valor < (*raiz)->info.numero)
        {
            if((*raiz)->esquerdo != NULL)
            {
                if(retornar_cor((*raiz)->esquerdo) == PRETO && retornar_cor((*raiz)->esquerdo->esquerdo) == PRETO)
                    mover_esquerda(raiz);
            }
                
            removeu = arvorevp_remover_no(&((*raiz)->esquerdo), valor);
        }
        else {
            if(retornar_cor((*raiz)->esquerdo) == VERMELHO)
                rotacao_direita(raiz);

            if(valor == (*raiz)->info.numero && (*raiz)->direito == NULL)
                no_desalocar(raiz);
            else
            {
                if((*raiz)->direito != NULL)
                {
                    if(retornar_cor((*raiz)->direito) == PRETO && retornar_cor((*raiz)->direito->esquerdo) == PRETO)
                        mover_direita(raiz);
                }

                if(valor == (*raiz)->info.numero)
                {
                    ArvoreVP *menor;
                    menor = menor_filho((*raiz)->direito);
                    (*raiz)->info = menor->info;
                    remover_menor(&((*raiz)->direito));
                    printf("\n");
                }
                else
                    removeu = arvorevp_remover_no(&((*raiz)->direito), valor);
            }
        }
        balancear_vp(raiz);
    }
    else
        removeu = 0;

    return removeu;
}

int arvorevp_inserir(ArvoreVP **raiz, Data info)
{
    int inseriu = arvorevp_inserir_no(raiz, info);

    if(*raiz != NULL)
        (*raiz)->cor = PRETO;

    return inseriu;
}

int arvorevp_remover(ArvoreVP **raiz, int valor)
{
    int removeu = arvorevp_remover_no(raiz, valor);
    
    if(*raiz != NULL)
        (*raiz)->cor = PRETO;

    return removeu;
}

int main()
{
    int tam, tam_remov;

    // int valores[] = {1000, 1500, 500, 2000, 2500, 750, 600, 400, 300, 550, 800};
    int valores[] = {1000, 4000, 3000, 2000, 500, 4500, 3500};
    int removidos[] = {3500, 2000, 3000, 4};

    tam = sizeof(valores) / sizeof(int);
    tam_remov = sizeof(removidos) / sizeof(int);

    ArvoreVP *arvore;
    arvore = arvorevp_criar();

    for(int i = 0; i < tam; i++)
    {
        Data info;
        info.numero = valores[i];
        
        arvorevp_inserir(&arvore, info);
    }
    printf("\n\nÁrvore após inserção: \n");
    arvorevp_exibir_pre(arvore);

    for(int i = 0; i < tam_remov; i++)
    {
        arvorevp_remover(&arvore, removidos[i]);
        printf("\n\nÁrvore após remover %d:\n", removidos[i]);
        arvorevp_exibir_pre(arvore);
    }

    printf("\n\n");
    return 0;
}
