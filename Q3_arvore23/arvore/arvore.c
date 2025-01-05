#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

int eh_folha(Arvore23 no)
{
    return no.esquerdo == NULL;
}

static int eh_info1(Arvore23 no, int info)
{
    return info == no.info1.endereco_inicial;
}

static int eh_info2(Arvore23 no, int info)
{
    return no.n_infos == 2 && info == no.info2.endereco_inicial;
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
    free(*no);
    *no = NULL;
}

Arvore23 *no23_criar(Data info, Arvore23 *filho_esquerdo, Arvore23 *filho_centro)
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

static Arvore23 *no23_quebrar(Arvore23 *no, Data info, Data *promove, Arvore23 *filho_maior)
{
    Arvore23 *maior;
    if(info.endereco_inicial > no->info2.endereco_inicial)
    {
        *promove = no->info2;
        maior = no23_criar(info, no->direito, filho_maior);
    }
    else if(info.endereco_inicial > no->info1.endereco_inicial)
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


static void no23_adicionar_info(Arvore23 *no, Data info, Arvore23 *filho_maior)
{
    if(info.endereco_inicial > no->info1.endereco_inicial)
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

static Arvore23 *no23_juntar(Arvore23 *filho1, Data info, Arvore23 *maior, Arvore23 **raiz)
{
    no23_adicionar_info(filho1, info, maior);

    (*raiz)->n_infos--;

    if((*raiz)->n_infos == 0)
        no23_desalocar(raiz);

    return filho1;
}

Data *no23_maior_info(Arvore23 *raiz)
{
    return raiz->n_infos == 2 ? &(raiz->info2) : &(raiz->info1);
}

Arvore23 *arvore23_criar()
{
    return NULL;
}

Arvore23 *arvore23_buscar(Arvore23 *raiz, int info)
{
    Arvore23 *no;
    no = NULL;

    if(raiz != NULL)
    {
        if(eh_info1(*raiz, info) || eh_info2(*raiz, info))
            no = raiz;
        else if(info < raiz->info1.endereco_inicial)
            no = arvore23_buscar(raiz->esquerdo, info);
        else if(raiz->n_infos == 1 || info < raiz->info2.endereco_inicial)
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

Arvore23 *arvore23_buscar_maior_filho(Arvore23 *raiz, Arvore23 **pai, Data **maior_valor)
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

Arvore23 *arvore23_buscar_pai(Arvore23 *raiz, int info)
{
    Arvore23 *pai;
    pai = NULL;

    if(raiz != NULL)
    {
        if(!eh_info1(*raiz, info) && !eh_info2(*raiz, info))
        {
            if(info < raiz->info1.endereco_inicial)
                pai = arvore23_buscar_pai(raiz->esquerdo, info);
            else if(raiz->n_infos == 1 || info < raiz->info2.endereco_inicial)
                pai = arvore23_buscar_pai(raiz->centro, info);
            else
                pai = arvore23_buscar_pai(raiz->direito, info);

            if(pai == NULL)
                pai = raiz;
        }
    }

    return pai;
}

Arvore23 *arvore23_buscar_maior_pai(Arvore23 *raiz, int info)
{
    Arvore23 *pai;
    pai = NULL;

    if(raiz != NULL)
    {
        if(!eh_info1(*raiz, info) && !eh_info2(*raiz, info))
        {
            if(info < raiz->info1.endereco_inicial)
                pai = arvore23_buscar_maior_pai(raiz->esquerdo, info);
            else if(raiz->n_infos == 1 || info < raiz->info2.endereco_inicial)
                pai = arvore23_buscar_maior_pai(raiz->centro, info);
            else
                pai = arvore23_buscar_maior_pai(raiz->direito, info);

            if(pai == NULL && ((raiz->n_infos == 1 && raiz->info1.endereco_inicial > info) || (raiz->n_infos == 2 && raiz->info2.endereco_inicial > info)))
                pai = raiz;
        }
    }

    return pai;
}

Arvore23 *arvore23_buscar_menor_pai(Arvore23 *raiz, int info)
{
    Arvore23 *pai;
    pai = NULL;

    if(raiz != NULL)
    {
        if(!eh_info1(*raiz, info) && !eh_info2(*raiz, info))
        {
            if(info < raiz->info1.endereco_inicial)
                pai = arvore23_buscar_menor_pai(raiz->esquerdo, info);
            else if(raiz->n_infos == 1 || info < raiz->info2.endereco_inicial)
                pai = arvore23_buscar_menor_pai(raiz->centro, info);
            else
                pai = arvore23_buscar_menor_pai(raiz->direito, info);

            if(pai == NULL && raiz->info1.endereco_inicial < info)
                pai = raiz;
        }
    }

    return pai;
}

static Arvore23 *arvore23_buscar_menor_pai_2_infos(Arvore23 *raiz, int info)
{
    Arvore23 *pai;
    pai = NULL;

    if(raiz != NULL)
    {
        if(!eh_info1(*raiz, info) && !eh_info2(*raiz, info))
        {
            if(info < raiz->info1.endereco_inicial)
                pai = arvore23_buscar_menor_pai_2_infos(raiz->esquerdo, info);
            else if(raiz->n_infos == 1 || info < raiz->info2.endereco_inicial)
                pai = arvore23_buscar_menor_pai_2_infos(raiz->centro, info);
            else
                pai = arvore23_buscar_menor_pai_2_infos(raiz->direito, info);

            if(pai == NULL && raiz->n_infos == 2 && raiz->info2.endereco_inicial < info)
                pai = raiz;
        }
    }

    return pai;
}

static int movimento_onda(Data saindo, Data *entrada, Arvore23 *pai, Arvore23 **origem, Arvore23 **raiz, Arvore23 **maior, int (*funcao_remover)(Arvore23 **, int, Arvore23 *, Arvore23 **, Arvore23 **))
{
    int removeu = funcao_remover(raiz, saindo.endereco_inicial, pai, origem, maior);
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

Arvore23 *arvore23_inserir_no(Arvore23 **raiz, Data info, Arvore23 *pai, Data *promove)
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
            if(info.endereco_inicial < (*raiz)->info1.endereco_inicial)
                maior = arvore23_inserir_no(&((*raiz)->esquerdo), info, *raiz, promove);
            else if((*raiz)->n_infos == 1 || info.endereco_inicial < (*raiz)->info2.endereco_inicial)
                maior = arvore23_inserir_no(&((*raiz)->centro), info, *raiz, promove);
            else
                maior = arvore23_inserir_no(&((*raiz)->direito), info, *raiz, promove);

            if(maior != NULL)
            {
                if((*raiz)->n_infos == 1)
                {
                    no23_adicionar_info(*raiz, *promove, maior);
                    maior = NULL;
                }
                else
                {
                    Data promove_aux;
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

Arvore23 *arvore23_inserir(Arvore23 **raiz, Data info)
{
    Data promove;
    return arvore23_inserir_no(raiz, info, NULL, &promove);
}


static int arvore23_remover_no_interno1(Arvore23 **origem, Arvore23* raiz, Data *info, Arvore23 *filho1, Arvore23 *filho2, Arvore23 **maior)
{
    int removeu;
    Arvore23 *filho, *pai;
    Data *info_filho;

    pai = raiz;

    filho = arvore23_buscar_maior_filho(filho1, &pai, &info_filho);

    if(filho->n_infos == 2)
    {
        *info = *info_filho;
        filho->n_infos = 1;
    }
    else
    {
        filho = arvore23_buscar_menor_filho(filho2, &pai);
        removeu = movimento_onda(filho->info1, info, pai, origem, &filho, maior, arvore23_remover1);
    }

    return removeu;
}

static int arvore23_remover_no_interno2(Arvore23 **origem, Arvore23* raiz, Data *info, Arvore23 *filho1, Arvore23 *filho2, Arvore23 **maior)
{
    int removeu;
    Arvore23 *filho, *pai;
    Data *info_filho;

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
        removeu = movimento_onda(*info_filho, info, pai, origem, &filho, maior, arvore23_remover2);
    }

    return removeu;
}

int arvore23_remover1(Arvore23 **raiz, int info, Arvore23 *pai, Arvore23 **origem, Arvore23 **maior)
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
                        Data info_pai;
                        if(*raiz == pai->esquerdo || (pai->n_infos == 2 && *raiz == pai->centro))
                        {
                            pai_aux = arvore23_buscar_pai(*origem, pai->info1.endereco_inicial);
                            
                            if(*raiz == pai->esquerdo)
                                info_pai = pai->info1;
                            else 
                                info_pai = pai->info2;

                            removeu = movimento_onda(info_pai, &((*raiz)->info1), pai_aux, origem, &pai, maior, arvore23_remover1);
                        }
                        else // Filho do centro (com pai de 1 info) ou da direita
                        {
                            pai_aux = arvore23_buscar_maior_pai(*origem, (*raiz)->info1.endereco_inicial);

                            Arvore23 *menor_pai;
                            menor_pai = arvore23_buscar_menor_pai_2_infos(*origem, (*raiz)->info1.endereco_inicial);


                            if(pai_aux != NULL)
                            {
                                if(pai_aux->info1.endereco_inicial > (*raiz)->info1.endereco_inicial)
                                    info_pai = pai_aux->info1;
                                else
                                    info_pai = pai_aux->info2;
                            }

                            int altura_menor_pai = calcular_altura(menor_pai);
                            int altura_pai_aux = calcular_altura(pai_aux);

                            if(pai_aux == NULL || (pai_aux != pai && menor_pai != NULL && altura_menor_pai <= altura_pai_aux && info_pai.endereco_inicial > menor_pai->info2.endereco_inicial))
                            {
                                *maior = pai;
                                (*raiz)->n_infos = 0;
                                removeu = -1;
                            }
                            else
                            {

                                Arvore23 *avo;
                                avo = arvore23_buscar_pai(*origem, info_pai.endereco_inicial);
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
            if(info < (*raiz)->info1.endereco_inicial)
                removeu = arvore23_remover1(&(*raiz)->esquerdo, info, *raiz, origem, maior);
            else if((*raiz)->n_infos == 1 || info < (*raiz)->info2.endereco_inicial)
                removeu = arvore23_remover1(&(*raiz)->centro, info, *raiz, origem, maior);
            else
                removeu = arvore23_remover1(&(*raiz)->direito, info, *raiz, origem, maior);
        }
    }
    return removeu;
}

int arvore23_remover2(Arvore23 **raiz, int info, Arvore23 *pai, Arvore23 **origem, Arvore23 **maior)
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
                        Data info_pai;
                        if(*raiz == pai->centro || (pai->n_infos == 2 && *raiz == pai->direito))
                        {
                            pai_aux = arvore23_buscar_pai(*origem, pai->info1.endereco_inicial);
                            
                            if(*raiz == pai->centro)
                                info_pai = pai->info1;
                            else 
                                info_pai = pai->info2;

                            removeu = movimento_onda(info_pai, &((*raiz)->info1), pai_aux, origem, &pai, maior, arvore23_remover2);
                        }
                        else // Filho da esquerda
                        {
                            pai_aux = arvore23_buscar_menor_pai(*origem, (*raiz)->info1.endereco_inicial);

                            Arvore23 *menor_pai;
                            menor_pai = arvore23_buscar_menor_pai_2_infos(*origem, (*raiz)->info1.endereco_inicial);

                            Arvore23 *avo;
                            if(pai_aux == NULL || (pai_aux != pai && menor_pai != NULL))
                            {  
                                removeu = -1;
                                *maior = pai;
                            }
                            else
                            {
                                if(pai_aux->n_infos == 2 && pai_aux->info2.endereco_inicial < (*raiz)->info1.endereco_inicial)
                                    info_pai = pai_aux->info2;
                                else
                                    info_pai = pai_aux->info1;

                                avo = arvore23_buscar_pai(*origem, info_pai.endereco_inicial);
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
            if(info < (*raiz)->info1.endereco_inicial)
                removeu = arvore23_remover2(&(*raiz)->esquerdo, info, *raiz, origem, maior);
            else if((*raiz)->n_infos == 1 || info < (*raiz)->info2.endereco_inicial)
                removeu = arvore23_remover2(&(*raiz)->centro, info, *raiz, origem, maior);
            else
                removeu = arvore23_remover2(&(*raiz)->direito, info, *raiz, origem, maior);
        }
    }
    return removeu;
}

int arvore23_remover(Arvore23 **raiz, int info)
{   
    Arvore23 *maior, *posicao_juncao;
    int removeu = arvore23_remover1(raiz, info, NULL, raiz, &posicao_juncao);

    if(removeu == -1)
    {
        removeu = 1;
        Data valor_juncao = *(no23_maior_info(posicao_juncao));
        maior = NULL;
        int removeu_aux = arvore23_rebalancear(raiz, valor_juncao.endereco_inicial, &maior);
        
        if(removeu_aux == -1)
        {
            Arvore23 *pai, *posicao_juncao2;
            Data *entrada;
            pai = arvore23_buscar_pai(*raiz, valor_juncao.endereco_inicial);

            if(eh_info1(*posicao_juncao, valor_juncao.endereco_inicial))
                entrada = &(posicao_juncao->centro->info1);
            else
                entrada = &(posicao_juncao->direito->info1);

            removeu_aux = movimento_onda(valor_juncao, entrada, pai, raiz, &posicao_juncao, &posicao_juncao2, arvore23_remover2);

            if(removeu_aux == -1)
            {
                valor_juncao = posicao_juncao2->info1;
                pai = arvore23_buscar_pai(*raiz, valor_juncao.endereco_inicial);
                removeu_aux = movimento_onda(valor_juncao, &(posicao_juncao2->esquerdo->info1), pai, raiz, &posicao_juncao2, &posicao_juncao, arvore23_remover1);

                valor_juncao = *(no23_maior_info(posicao_juncao));
                maior = NULL;
                removeu_aux = arvore23_rebalancear(raiz, valor_juncao.endereco_inicial, &maior);
            }
        }

        if(*raiz == NULL)
            *raiz = maior;
    }

    return removeu;
}

static int balanceamento(Arvore23 **raiz, Arvore23 *filho1, Arvore23 **filho2, Data info, Arvore23 **maior)
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

int arvore23_rebalancear(Arvore23 **raiz, int info, Arvore23 **maior)
{
    int balanceou = 0;
    if(*raiz != NULL)
    {
        if(!eh_folha(**raiz))
        {
            if(info < (*raiz)->info1.endereco_inicial)
                balanceou = arvore23_rebalancear(&((*raiz)->esquerdo), info, maior);
            else if((*raiz)->n_infos == 1 || info < (*raiz)->info2.endereco_inicial)
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

void no23_exibir(Data no)
{
    printf("Bloco de [%d] até [%d] - [%s]\n", no.endereco_inicial, no.endereco_final, no.status == LIVRE ? "Livre" : "Ocupado");
}

void arvore23_exibir_pre(Arvore23 *raiz)
{
    if(raiz != NULL)
    {
        printf("[1º] ");
        no23_exibir(raiz->info1);
        if(raiz->n_infos == 2)
        {
            printf("[2º] ");
            no23_exibir(raiz->info2);
        }

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
        printf("[1º] ");
        no23_exibir(raiz->info1);
        arvore23_exibir_ordem(raiz->centro);

        if(raiz->n_infos == 2)
        {
            printf("[2º] ");
            no23_exibir(raiz->info2);
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

        printf("[1º] ");
        no23_exibir(raiz->info1);
        if(raiz->n_infos == 2)
        {
            printf("[2º] ");
            no23_exibir(raiz->info2);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
#define tamanhoVetor(v) (sizeof(v) / sizeof((v)[0]))

int *alocarCaso(int valores[],int tamanho){
    int *value;
    value = (int*)calloc(tamanho,sizeof(int));

    for(int i=0;i<tamanho;i++)
        value[i] = valores[i];

    return value;
}

int *caso_selecionado(int opc,int *tam){
    int *caso;

    if (opc == 1) {
        /*
            --Caso 1.2.3
        */
        int valores[] = {50, 100};
        *tam = tamanhoVetor(valores);
        caso = alocarCaso(valores, *tam);
    } else if (opc == 2) {
        /*
            --Caso 1.2.-3
        */
        int valores[] = {250, 100};
        *tam = tamanhoVetor(valores);
        caso = alocarCaso(valores, *tam);
    } else if (opc == 3) {
        /*
            --Caso 1.-2.3.4
        */
        int valores[] = {200, 100, 150};
        *tam = tamanhoVetor(valores);
        caso = alocarCaso(valores, *tam);
    } else if (opc == 4) {
        /*
            --Caso 1.-2.3.-4.5.6
        */
        int valores[] = {50, 100, 70, 200, 170};
        *tam = tamanhoVetor(valores);
        caso = alocarCaso(valores, *tam);
    } else if (opc == 5) {
        /*
            --Caso 1.-2.3.-4.5.-6
        */
        int valores[] = {50, 70, 60, 80, 100};
        *tam = tamanhoVetor(valores);
        caso = alocarCaso(valores, *tam);
    } else if (opc == 6) {
        /*
            --Caso 1.-2.3.-4.-5
        */
        int valores[] = {50, 70, 100};
        *tam = tamanhoVetor(valores);
        caso = alocarCaso(valores, *tam);
    } else if (opc == 7) {
        /*
            --Caso 1.-2.-3
        */
        int valores[] = {100};
        *tam = tamanhoVetor(valores);
        caso = alocarCaso(valores, *tam);
    } else if (opc == 8) {
        /*
            --Caso -1.2.3
        */
        int valores[] = {10, 50, 60, 30, 100, 150, 200, 80};
        *tam = tamanhoVetor(valores);
        caso = alocarCaso(valores, *tam);
    } else if (opc == 9) {
        /*
            --Caso -1.2.-3.4
        */
        int valores[] = {10, 50, 60, 30, 100, 150, 80};
        *tam = tamanhoVetor(valores);
        caso = alocarCaso(valores, *tam);
    } else if (opc == 10) {
        /*
            --Caso -1.2.-3.-4
        */
        int valores[] = {10, 50, 60, 30, 100, 150};
        *tam = tamanhoVetor(valores);
        caso = alocarCaso(valores, *tam);
    } else if (opc == 11) {
        /*
            --Caso -1.-2.3
        */
        int valores[] = {30, 120, 100, 170, 150, 200, 140, 50};
        *tam = tamanhoVetor(valores);
        caso = alocarCaso(valores, *tam);
    } else if (opc == 12) {
        /*
            --Caso -1.-2.-3.4
        */
        int valores[] = {30, 120, 100, 170, 150, 200, 140};
        *tam = tamanhoVetor(valores);
        caso = alocarCaso(valores, *tam);
    } else if (opc == 13) {
        /*
            --Caso -1.-2.-3.-4.5.6
        */
        int valores[] = {30, 120, 100, 170, 150, 200};
        *tam = tamanhoVetor(valores);
        caso = alocarCaso(valores, *tam);
    } else if (opc == 14) {
        /*
            --Caso -1.-2.-3.-4.5.-6
        */
        int valores[] = {30, 120, 100, 170, 150};
        *tam = tamanhoVetor(valores);
        caso = alocarCaso(valores, *tam);
    } else if (opc == 15) {
        /*
            --Caso -1.-2.-3.-4.-5.-6
        */
        int valores[] = {30, 120, 100};
        *tam = tamanhoVetor(valores);
        caso = alocarCaso(valores, *tam);
    } else if (opc == 16) {
        /*
            --Caso -1.-2.-3.-4.-5.6.7
        */
        int valores[] = {50, 150, 100, 200, 300, 350, 400, 500, 600, 450};
        *tam = tamanhoVetor(valores);
        caso = alocarCaso(valores, *tam);
    } else if (opc == 17) {
        /*
            --Caso -1.-2.-3.-4.-5.6.-7.8.9
        */
        int valores[] = {20,70,100,300,200,10,30,40,50,60,80,120,350,250,270};
        *tam = tamanhoVetor(valores);
        caso = alocarCaso(valores, *tam);
    } else if (opc == 18) {
        /*
            --Caso -1.-2.-3.-4.-5.6.-7.8.-9
        */
        int valores[] = {10,30,40,70,100,5,15,20,25,35,45,60,80,150};
        *tam = tamanhoVetor(valores);
        caso = alocarCaso(valores, *tam);

    } else if (opc == 19) {
        /*
            Caso -1.-2.-3.-4.-5.6.-7.-8
        */
        int valores[] = {10,30,20,70,80,100,120,40,50,60};
        *tam = tamanhoVetor(valores);
        caso = alocarCaso(valores, *tam);
    }
    
    return caso;
}
/////////////////////////////////////////////////////////////////////////////////////////////

int main1()
{
    int tam;

    // int valores[] = {5000, 4000, 1000, 2000, 1500, 500, 300, 6000, 8000, 7000};
    int valores[] = {8000, 10000, 15000, 1000, 3000, 7000, 5800, 4200, 2500, 1800, 9000, 7500, 6500, 4300, 3500, 9500, 2100, 500, 900, 100, 600, 1700, 2400, 1250, 1750, 250};
    // int valores[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'x', 'w', 'y', 'z'};
    // int valores[] = {30, 120, 100, 50, 170, 150, 140, 200};
    // int valores[] = {2, 4, 7, 3, 1, 6, 5};
    tam = sizeof(valores) / sizeof(int);

    Arvore23 *arvore, *arvore2;
    arvore = arvore23_criar();
    arvore2 = arvore23_criar();

    for(int i = 0; i < tam; i++)
    {
        Data info;
        info.endereco_inicial = valores[i];
        
        arvore23_inserir(&arvore, info);
        arvore23_inserir(&arvore2, info);
    }

    printf("\n\nÁrvore após inserção: \n");
    arvore23_exibir_pre(arvore);

    for(int i = tam-1; i >= 0; i--)
    {
        arvore23_remover(&arvore, valores[i]);
        printf("\n\nÁrvore após remover %d:\n", valores[i]);
        arvore23_exibir_pre(arvore);
    }

    printf("\n\nÁrvore após inserção: \n");
    arvore23_exibir_pre(arvore2);

    for(int i = 0; i < tam; i++)
    {
        arvore23_remover(&arvore2, valores[i]);
        printf("\n\nÁrvore após remover %d:\n", valores[i]);
        arvore23_exibir_pre(arvore2);
    }

    arvore23_desalocar(&arvore);

    printf("\n\n");
    return 0;
}

int main2()
{
    int tam, valor = 100;
    int *valores;

    Arvore23 *arvore;
    arvore = arvore23_criar();

    for(int i = 1; i <= 19; i++)
    {
        printf("\n\n[%dº caso]", i);

        valores = caso_selecionado(i, &tam);
        for(int j = 0; j < tam; j++)
        {
            Data info;
            info.endereco_inicial = valores[j];
            arvore23_inserir(&arvore, info);
        }
        printf("\n\nÁrvore após inserção: \n");
        arvore23_exibir_pre(arvore);

        arvore23_remover(&arvore, valor);

        printf("\n\nÁrvore após remover %d:\n", valor);
        arvore23_exibir_pre(arvore);
        printf("\n\n");

        arvore23_desalocar(&arvore);
    }
    return 0;
}

// int main()
// {
//     main1();
//     main2();

//     return 0;
// }