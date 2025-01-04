#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"

static int eh_folha(Arvore23 no)
{
    return no.esquerdo == NULL;
}

static int eh_info1(Arvore23 no, char *info)
{
    return strcmp(info, no.info1.palavraPT) == 0;
}

static int eh_info2(Arvore23 no, char *info)
{
    return no.n_infos == 2 && (strcmp(info, no.info2.palavraPT) == 0);
}

static int calcular_altura(Arvore23 *no)
{
    int altura = -1;

    if(no != NULL)
        altura = 1 + calcular_altura(no->esquerdo);

    return altura;
}

static int possivel_remover(Arvore23 *raiz)
{
    int possivel = 0;

    if(raiz != NULL)
    {
        possivel = raiz->n_infos == 2;

        if(!possivel)
        {
            possivel = possivel_remover(raiz->centro);

            if(!possivel)
                possivel = possivel_remover(raiz->esquerdo);
        }
    }

    return possivel;
}

Arvore23 *no23_alocar()
{
    Arvore23 *no;
    no = (Arvore23 *)malloc(sizeof(Arvore23));

    if (!no)
    {
        printf("Erro ao alocar nó da árvore");
        exit(EXIT_FAILURE);
    }

    return no;
}

void no23_desalocar(Arvore23 **no)
{
    if(*no != NULL)
    {
        free((*no)->info1.palavraPT);
        (*no)->info1.palavraPT = NULL;

        arvorebb_desalocar(&((*no)->info1.palavrasEng));

        free(*no);
        *no = NULL;
    }
}

Arvore23 *no23_criar(DataPT info, Arvore23 *filho_esquerdo, Arvore23 *filho_centro)
{
    Arvore23 *no;
    no = no23_alocar();

    no->info1 = info;
    no->esquerdo = filho_esquerdo;
    no->centro = filho_centro;
    no->direito = NULL;
    no->n_infos = 1;
    return no;
}

static Arvore23 *no23_quebrar(Arvore23 *no, DataPT info, DataPT *promove, Arvore23 *filho_maior)
{
    Arvore23 *maior;
    if(strcmp(info.palavraPT, no->info2.palavraPT) > 0)
    {
        *promove = no->info2;
        maior = no23_criar(info, no->direito, filho_maior);
    }
    else if(strcmp(info.palavraPT, no->info1.palavraPT) > 0)
    {
        *promove = info;
        maior = no23_criar(no->info2, filho_maior, no->direito);
    }
    else
    {
        *promove = no->info1;
        maior = no23_criar(no->info2, no->centro, no->direito);
        no->info1 = info;
        no->centro = filho_maior;
    }
    no->n_infos = 1;

    return maior;
}


static void no23_adicionar_info(Arvore23 *no, DataPT info, Arvore23 *filho_maior)
{
    if(strcmp(info.palavraPT, no->info1.palavraPT) > 0)
    {
        no->info2 = info;
        no->direito = filho_maior;
    }
    else
    {
        no->info2 = no->info1;
        no->direito = no->centro;
        no->centro = filho_maior;
        no->info1 = info;
    }
    no->n_infos = 2;
}

static Arvore23 *no23_juntar(Arvore23 *filho1, DataPT info, Arvore23 *maior, Arvore23 **raiz)
{
    no23_adicionar_info(filho1, info, maior);

    (*raiz)->n_infos--;

    if((*raiz)->n_infos == 0)
        no23_desalocar(raiz);

    return filho1;
}

DataPT no23_maior_info(Arvore23 *raiz)
{
    return raiz->n_infos == 2 ? raiz->info2 : raiz->info1;
}

Arvore23 *arvore23_criar()
{
    return NULL;
}

Arvore23 *arvore23_buscar(Arvore23 *raiz, char *info)
{
    Arvore23 *no;
    no = NULL;

    if(raiz != NULL)
    {
        if(eh_info1(*raiz, info) || eh_info2(*raiz, info))
            no = raiz;
        else if(strcmp(info, raiz->info1.palavraPT) < 0)
            no = arvore23_buscar(raiz->esquerdo, info);
        else if(raiz->n_infos == 1 || (strcmp(info, raiz->info2.palavraPT) < 0))
            no = arvore23_buscar(raiz->centro, info);
        else
            no = arvore23_buscar(raiz->direito, info);
    }

    return no;
}

Arvore23 *arvore23_buscar_menor_filho(Arvore23 *raiz, Arvore23 **pai)
{
    Arvore23 *filho;
    filho = raiz;

    while(!eh_folha(*filho))
    {
        *pai = filho;
        filho = filho->esquerdo;
    }

    return filho;
}

Arvore23 *arvore23_buscar_maior_filho(Arvore23 *raiz, Arvore23 **pai, DataPT *maior_valor)
{
    Arvore23 *filho;
    filho = raiz;

    while(!eh_folha(*filho))
    {
        *pai = filho;
        if(filho->n_infos == 1)
            filho = filho->centro;
        else
            filho = filho->direito;
    }

    if(filho != NULL)
        *maior_valor = no23_maior_info(filho);

    return filho;
}

Arvore23 *arvore23_buscar_pai(Arvore23 *raiz, char *info)
{
    Arvore23 *pai;
    pai = NULL;

    if(raiz != NULL)
    {
        if(!eh_info1(*raiz, info) && !eh_info2(*raiz, info))
        {
            if(strcmp(info, raiz->info1.palavraPT) < 0)
                pai = arvore23_buscar_pai(raiz->esquerdo, info);
            else if(raiz->n_infos == 1 || (strcmp(info, raiz->info2.palavraPT) < 0))
                pai = arvore23_buscar_pai(raiz->centro, info);
            else
                pai = arvore23_buscar_pai(raiz->direito, info);

            if(pai == NULL)
                pai = raiz;
        }
    }

    return pai;
}

Arvore23 *arvore23_buscar_maior_pai(Arvore23 *raiz, char *info)
{
    Arvore23 *pai;
    pai = NULL;

    if(raiz != NULL)
    {
        if(!eh_info1(*raiz, info) && !eh_info2(*raiz, info))
        {
            if(strcmp(info, raiz->info1.palavraPT) < 0)
                pai = arvore23_buscar_maior_pai(raiz->esquerdo, info);
            else if(raiz->n_infos == 1 || (strcmp(info, raiz->info2.palavraPT) < 0))
                pai = arvore23_buscar_maior_pai(raiz->centro, info);
            else
                pai = arvore23_buscar_maior_pai(raiz->direito, info);

            if(pai == NULL &&
            ((raiz->n_infos == 1 && (strcmp(raiz->info1.palavraPT, info) > 0)) || (raiz->n_infos == 2 && (strcmp(raiz->info2.palavraPT, info) > 0))))
                pai = raiz;
        }
    }

    return pai;
}

Arvore23 *arvore23_buscar_menor_pai(Arvore23 *raiz, char *info)
{
    Arvore23 *pai;
    pai = NULL;

    if(raiz != NULL)
    {
        if(!eh_info1(*raiz, info) && !eh_info2(*raiz, info))
        {
            if(strcmp(info, raiz->info1.palavraPT) < 0)
                pai = arvore23_buscar_menor_pai(raiz->esquerdo, info);
            else if(raiz->n_infos == 1 || (strcmp(info, raiz->info2.palavraPT) < 0))
                pai = arvore23_buscar_menor_pai(raiz->centro, info);
            else
                pai = arvore23_buscar_menor_pai(raiz->direito, info);

            if(pai == NULL && (strcmp(raiz->info1.palavraPT, info) < 0))
                pai = raiz;
        }
    }

    return pai;
}

static Arvore23 *arvore23_buscar_menor_pai_2_infos(Arvore23 *raiz, char *info)
{
    Arvore23 *pai;
    pai = NULL;

    if(raiz != NULL)
    {
        if(!eh_info1(*raiz, info) && !eh_info2(*raiz, info))
        {
            if(strcmp(info, raiz->info1.palavraPT) < 0)
                pai = arvore23_buscar_menor_pai_2_infos(raiz->esquerdo, info);
            else if(raiz->n_infos == 1 || (strcmp(info, raiz->info2.palavraPT) < 0))
                pai = arvore23_buscar_menor_pai_2_infos(raiz->centro, info);
            else
                pai = arvore23_buscar_menor_pai_2_infos(raiz->direito, info);

            if(pai == NULL && raiz->n_infos == 2 && (strcmp(raiz->info2.palavraPT, info) < 0))
                pai = raiz;
        }
    }

    return pai;
}

static int movimento_onda(DataPT saindo, DataPT *entrada, Arvore23 *pai, Arvore23 **origem, Arvore23 **raiz, Arvore23 **maior, int (*funcao_remover)(Arvore23 **, char *, Arvore23 *, Arvore23 **, Arvore23 **))
{
    int removeu = funcao_remover(raiz, saindo.palavraPT, pai, origem, maior);
    *entrada = saindo;
    return removeu;
}


void arvore23_desalocar(Arvore23 **raiz)
{
    if(*raiz != NULL)
    {
        arvore23_desalocar(&((*raiz)->esquerdo));
        arvore23_desalocar(&((*raiz)->centro));

        if((*raiz)->n_infos == 2)
            arvore23_desalocar(&((*raiz)->direito));

        no23_desalocar(raiz);
    }
}

Arvore23 *arvore23_inserir(Arvore23 **raiz, DataPT info, Arvore23 *pai, DataPT *promove)
{
    Arvore23 *maior;
    maior = NULL;

    if(*raiz == NULL)
        *raiz = no23_criar(info, NULL, NULL);
    else
    {
        if(eh_folha(**raiz))
        {
            if((*raiz)->n_infos == 1)
                no23_adicionar_info(*raiz, info, NULL);
            else
            {
                maior = no23_quebrar(*raiz, info, promove, NULL);
                if(pai == NULL)
                {
                    *raiz = no23_criar(*promove, *raiz, maior);
                    maior = NULL;
                }
            }
        }
        else
        {
            if(strcmp(info.palavraPT, (*raiz)->info1.palavraPT) < 0)
                maior = arvore23_inserir(&((*raiz)->esquerdo), info, *raiz, promove);
            else if((*raiz)->n_infos == 1 || (strcmp(info.palavraPT, (*raiz)->info2.palavraPT) < 0))
                maior = arvore23_inserir(&((*raiz)->centro), info, *raiz, promove);
            else
                maior = arvore23_inserir(&((*raiz)->direito), info, *raiz, promove);

            if(maior != NULL)
            {
                if((*raiz)->n_infos == 1)
                {
                    no23_adicionar_info(*raiz, *promove, maior);
                    maior = NULL;
                }
                else
                {
                    DataPT promove_aux;
                    maior = no23_quebrar(*raiz, *promove, &promove_aux, maior);
                    *promove = promove_aux;
                    if(pai == NULL)
                    {
                        *raiz = no23_criar(promove_aux, *raiz, maior);
                        maior = NULL;
                    }
                }
            }
        }
    }

    return maior;
}

static int arvore23_remover_no_interno1(Arvore23 **origem, Arvore23* raiz, DataPT *info, Arvore23 *filho1, Arvore23 *filho2, Arvore23 **maior)
{
    int removeu;
    Arvore23 *filho, *pai;
    DataPT info_filho;

    pai = raiz;

    filho = arvore23_buscar_maior_filho(filho1, &pai, &info_filho);

    if(filho->n_infos == 2)
    {
        *info = info_filho;
        filho->n_infos = 1;
    }
    else
    {
        filho = arvore23_buscar_menor_filho(filho2, &pai);
        removeu = movimento_onda(filho->info1, info, pai, origem, &filho, maior, arvore23_remover1);
    }

    return removeu;
}

static int arvore23_remover_no_interno2(Arvore23 **origem, Arvore23* raiz, DataPT *info, Arvore23 *filho1, Arvore23 *filho2, Arvore23 **maior)
{
    int removeu;
    Arvore23 *filho, *pai;
    DataPT info_filho;

    pai = raiz;

    filho = arvore23_buscar_menor_filho(filho1, &pai);

    if(filho->n_infos == 2)
    {
        *info = filho->info1;
        filho->info1 = filho->info2;
        filho->n_infos = 1;
    }
    else
    {
        filho = arvore23_buscar_maior_filho(filho2, &pai, &info_filho);
        removeu = movimento_onda(info_filho, info, pai, origem, &filho, maior, arvore23_remover2);
    }

    return removeu;
}

int arvore23_remover1(Arvore23 **raiz, char *info, Arvore23 *pai, Arvore23 **origem, Arvore23 **maior)
{
    int removeu = 0;

    if(*raiz != NULL)
    {
        int info1 = eh_info1(**raiz, info);
        int info2 = eh_info2(**raiz, info);

        if(info1 || info2)
        {
            removeu = 1;
            if(eh_folha(**raiz))
            {
                if((*raiz)->n_infos == 2)
                {
                    if(info1)
                        (*raiz)->info1 = (*raiz)->info2;

                    (*raiz)->n_infos = 1;
                }
                else
                {
                    if(pai == NULL)
                        no23_desalocar(raiz);
                    else
                    {
                        Arvore23 *pai_aux;
                        DataPT info_pai;
                        if(*raiz == pai->esquerdo || (pai->n_infos == 2 && *raiz == pai->centro))
                        {
                            pai_aux = arvore23_buscar_pai(*origem, pai->info1.palavraPT);
                            
                            if(*raiz == pai->esquerdo)
                                info_pai = pai->info1;
                            else 
                                info_pai = pai->info2;

                            removeu = movimento_onda(info_pai, &((*raiz)->info1), pai_aux, origem, &pai, maior, arvore23_remover1);
                        }
                        else // Filho do centro (com pai de 1 info) ou da direita
                        {
                            pai_aux = arvore23_buscar_maior_pai(*origem, (*raiz)->info1.palavraPT);

                            Arvore23 *menor_pai;
                            menor_pai = arvore23_buscar_menor_pai_2_infos(*origem, (*raiz)->info1.palavraPT);


                            if(pai_aux != NULL)
                            {
                                if(strcmp(pai_aux->info1.palavraPT, (*raiz)->info1.palavraPT) > 0)
                                    info_pai = pai_aux->info1;
                                else
                                    info_pai = pai_aux->info2;
                            }

                            int altura_menor_pai = calcular_altura(menor_pai);
                            int altura_pai_aux = calcular_altura(pai_aux);

                            if(pai_aux == NULL || (pai_aux != pai && menor_pai != NULL && altura_menor_pai <= altura_pai_aux && (strcmp(info_pai.palavraPT, menor_pai->info2.palavraPT) > 0)))
                            {
                                *maior = pai;
                                (*raiz)->n_infos = 0;
                                removeu = -1;
                            }
                            else
                            {

                                Arvore23 *avo;
                                avo = arvore23_buscar_pai(*origem, info_pai.palavraPT);
                                removeu = movimento_onda(info_pai, &((*raiz)->info1), avo, origem, &pai_aux, maior, arvore23_remover1);
                            }
                        }
                    }
                }
            }
            else if(info2)
                removeu = arvore23_remover_no_interno1(origem, *raiz, &((*raiz)->info2), (*raiz)->centro, (*raiz)->direito, maior);
            else if(info1)
                removeu = arvore23_remover_no_interno1(origem, *raiz, &((*raiz)->info1), (*raiz)->esquerdo, (*raiz)->centro, maior);
        }
        else
        {
            if(strcmp(info, (*raiz)->info1.palavraPT) < 0)
                removeu = arvore23_remover1(&(*raiz)->esquerdo, info, *raiz, origem, maior);
            else if((*raiz)->n_infos == 1 || (strcmp(info, (*raiz)->info2.palavraPT) < 0))
                removeu = arvore23_remover1(&(*raiz)->centro, info, *raiz, origem, maior);
            else
                removeu = arvore23_remover1(&(*raiz)->direito, info, *raiz, origem, maior);
        }
    }
    return removeu;
}

int arvore23_remover2(Arvore23 **raiz, char *info, Arvore23 *pai, Arvore23 **origem, Arvore23 **maior)
{
    int removeu = 0;

    if(*raiz != NULL)
    {
        int info1 = eh_info1(**raiz, info);
        int info2 = eh_info2(**raiz, info);

        if(info1 || info2)
        {
            removeu = 1;
            if(eh_folha(**raiz))
            {
                if((*raiz)->n_infos == 2)
                {
                    if(info1)
                        (*raiz)->info1 = (*raiz)->info2;

                    (*raiz)->n_infos = 1;
                }
                else
                {
                    if(pai == NULL)
                        no23_desalocar(raiz);
                    else
                    {
                        Arvore23 *pai_aux;
                        DataPT info_pai;
                        if(*raiz == pai->centro || (pai->n_infos == 2 && *raiz == pai->direito))
                        {
                            pai_aux = arvore23_buscar_pai(*origem, pai->info1.palavraPT);
                            
                            if(*raiz == pai->centro)
                                info_pai = pai->info1;
                            else 
                                info_pai = pai->info2;

                            removeu = movimento_onda(info_pai, &((*raiz)->info1), pai_aux, origem, &pai, maior, arvore23_remover2);
                        }
                        else // Filho da esquerda
                        {
                            pai_aux = arvore23_buscar_menor_pai(*origem, (*raiz)->info1.palavraPT);

                            Arvore23 *menor_pai;
                            menor_pai = arvore23_buscar_menor_pai_2_infos(*origem, (*raiz)->info1.palavraPT);

                            Arvore23 *avo;
                            if(pai_aux == NULL || (pai_aux != pai && menor_pai != NULL))
                            {  
                                removeu = -1;
                                *maior = pai;
                            }
                            else
                            {
                                if(pai_aux->n_infos == 2 && (strcmp(pai_aux->info2.palavraPT, (*raiz)->info1.palavraPT) < 0))
                                    info_pai = pai_aux->info2;
                                else
                                    info_pai = pai_aux->info1;

                                avo = arvore23_buscar_pai(*origem, info_pai.palavraPT);
                                removeu = movimento_onda(info_pai, &((*raiz)->info1), avo, origem, &pai_aux, maior, arvore23_remover2);
                            }
                        }
                    }
                }
            }
            else if(info2)
                removeu = arvore23_remover_no_interno2(origem, *raiz, &((*raiz)->info2), (*raiz)->direito, (*raiz)->centro, maior);
            else if(info1)
                removeu = arvore23_remover_no_interno2(origem, *raiz, &((*raiz)->info1), (*raiz)->centro, (*raiz)->esquerdo, maior);
        }
        else
        {
            if(strcmp(info, (*raiz)->info1.palavraPT) < 0)
                removeu = arvore23_remover2(&(*raiz)->esquerdo, info, *raiz, origem, maior);
            else if((*raiz)->n_infos == 1 || (strcmp(info, (*raiz)->info2.palavraPT) < 0))
                removeu = arvore23_remover2(&(*raiz)->centro, info, *raiz, origem, maior);
            else
                removeu = arvore23_remover2(&(*raiz)->direito, info, *raiz, origem, maior);
        }
    }
    return removeu;
}

int arvore23_remover(Arvore23 **raiz, char *info)
{   
    Arvore23 *maior, *posicao_juncao;
    int removeu = arvore23_remover1(raiz, info, NULL, raiz, &posicao_juncao);

    if(removeu == -1)
    {
        removeu = 1;
        DataPT valor_juncao = no23_maior_info(posicao_juncao);
        maior = NULL;
        int removeu_aux = arvore23_rebalancear(raiz, valor_juncao.palavraPT, &maior);
        
        if(removeu_aux == -1)
        {
            Arvore23 *pai, *posicao_juncao2;
            DataPT *entrada;
            pai = arvore23_buscar_pai(*raiz, valor_juncao.palavraPT);

            if(eh_info1(*posicao_juncao, valor_juncao.palavraPT))
                entrada = &(posicao_juncao->centro->info1);
            else
                entrada = &(posicao_juncao->direito->info1);

            removeu_aux = movimento_onda(valor_juncao, entrada, pai, raiz, &posicao_juncao, &posicao_juncao2, arvore23_remover2);

            if(removeu_aux == -1)
            {
                valor_juncao = posicao_juncao2->info1;
                pai = arvore23_buscar_pai(*raiz, valor_juncao.palavraPT);
                removeu_aux = movimento_onda(valor_juncao, &(posicao_juncao2->esquerdo->info1), pai, raiz, &posicao_juncao2, &posicao_juncao, arvore23_remover1);

                valor_juncao = no23_maior_info(posicao_juncao);
                maior = NULL;
                removeu_aux = arvore23_rebalancear(raiz, valor_juncao.palavraPT, &maior);
            }
        }

        if(*raiz == NULL)
            *raiz = maior;
    }

    return removeu;
}

static int balanceamento(Arvore23 **raiz, Arvore23 *filho1, Arvore23 **filho2, DataPT info, Arvore23 **maior)
{
    int balanceou = 0;
    if(*filho2 == NULL || (*filho2)->n_infos == 0)
    {
        if(*filho2 != NULL)
            no23_desalocar(filho2);

        *maior = no23_juntar(filho1, info, *maior, raiz);
        balanceou = 1;
    }
    return balanceou;
}

int arvore23_rebalancear(Arvore23 **raiz, char *info, Arvore23 **maior)
{
    int balanceou = 0;
    if(*raiz != NULL)
    {
        if(!eh_folha(**raiz))
        {
            if(strcmp(info, (*raiz)->info1.palavraPT) < 0)
                balanceou = arvore23_rebalancear(&((*raiz)->esquerdo), info, maior);
            else if((*raiz)->n_infos == 1 || (strcmp(info, (*raiz)->info2.palavraPT) < 0))
            {
                if((*raiz)->esquerdo->n_infos == 2 && !possivel_remover((*raiz)->centro))
                    balanceou = -1;
                else
                    balanceou = arvore23_rebalancear(&((*raiz)->centro), info, maior);
            }
            else
            {
                if((*raiz)->centro->n_infos == 2 && !possivel_remover((*raiz)->direito))
                    balanceou = -1;
                else
                    balanceou = arvore23_rebalancear(&((*raiz)->direito), info, maior);
            }

            if(balanceou != -1)
            {
                if((*raiz)->n_infos == 1)
                    balanceou = balanceamento(raiz, (*raiz)->esquerdo, &((*raiz)->centro), (*raiz)->info1, maior);
                else if((*raiz)->n_infos == 2)
                    balanceou = balanceamento(raiz, (*raiz)->centro, &((*raiz)->direito), (*raiz)->info2, maior);
            }
            
        }
    }

    return balanceou;
}

void arvore23_exibir_pre(Arvore23 *raiz)
{
    if(raiz != NULL)
    {
        printf("[1º] %s -> ", raiz->info1.palavraPT);
        if(raiz->n_infos == 2)
            printf("[2º] %s -> ", raiz->info2.palavraPT);

        arvore23_exibir_pre(raiz->esquerdo);
        arvore23_exibir_pre(raiz->centro);
        if(raiz->n_infos == 2)
            arvore23_exibir_pre(raiz->direito);
    }
}

void arvore23_exibir_ordem(Arvore23 *raiz)
{
    if(raiz != NULL)
    {
        arvore23_exibir_ordem(raiz->esquerdo);
        printf("[1º] %s -> ", raiz->info1.palavraPT);
        arvore23_exibir_ordem(raiz->centro);

        if(raiz->n_infos == 2)
        {
            printf("[2º] %s -> ", raiz->info2.palavraPT);
            arvore23_exibir_ordem(raiz->direito);
        }
    }
}

void arvore23_exibir_pos(Arvore23 *raiz)
{
    if(raiz != NULL)
    {
        arvore23_exibir_pos(raiz->esquerdo);
        arvore23_exibir_pos(raiz->centro);
        if(raiz->n_infos == 2)
            arvore23_exibir_pos(raiz->direito);

        printf("[1º] %s -> ", raiz->info1.palavraPT);
        if(raiz->n_infos == 2)
            printf("[2º] %s -> ", raiz->info2.palavraPT);
    }
}