#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

int eh_folha(Arvore23 no)
{
    return no.esquerdo == NULL;
}

int eh_info1(Arvore23 no, int info)
{
    return info == no.info1.numero;
}

int eh_info2(Arvore23 no, int info)
{
    return no.n_infos == 2 && info == no.info2.numero;
}

void troca_infos(Data *info1, Data *info2)
{
    Data aux = *info1;
    *info1 = *info2;
    *info2 = aux;
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

// TODO na folha o Nó possui **
Arvore23 *no23_quebrar(Arvore23 *no, Data info, Data *promove, Arvore23 *filho_maior)
{
    Arvore23 *maior;
    if(info.numero > no->info2.numero)
    {
        *promove = no->info2;
        maior = no23_criar(info, no->direito, filho_maior);
    }
    else if(info.numero > no->info1.numero)
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

Arvore23 *no23_juntar(Arvore23 *filho1, Arvore23 *filho2, Arvore23 **filho3)
{
    Arvore23 *maior;
    maior = NULL;

    if(!eh_folha(*filho2))
    {
        maior = no23_juntar(filho2->esquerdo, filho2->centro, &(filho1->direito));

        Data aux = filho2->info1;
        filho2->info1 = maior->info1;
        maior->info1 = aux;
    }

    filho1->info2 = filho2->info1;
    filho1->n_infos = 2;
    maior = filho1;
    *filho3 = maior;
    no23_desalocar(&filho2);
    
    return maior;
}

// TODO na folha há um retorno de nó
void no23_adicionar_info(Arvore23 *no, Data info, Arvore23 *filho_maior)
{
    if(info.numero > no->info1.numero)
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

Arvore23 *buscar_menor_filho(Arvore23 *raiz, Arvore23 **pai, Data *menor_info)
{
    Arvore23 *filho;
    filho = raiz;

    while(!eh_folha(*filho))
    {
        *pai = filho;
        filho = filho->esquerdo;
    }

    if(filho != NULL)
        *menor_info = filho->info1;

    
    return filho;
}

Arvore23 *maior_filho(Arvore23 *raiz)
{
    return raiz->n_infos == 2 ? raiz->direito : raiz->centro;
}

Arvore23 *buscar_maior_filho(Arvore23 *raiz, Arvore23 **pai, Data *maior_info)
{
    Arvore23 *filho;
    filho = raiz;

    while(!eh_folha(*filho))
    {
        *pai = filho;
        filho = maior_filho(filho);
    }

    if(filho != NULL)
        *maior_info = filho->n_infos == 2 ? filho->info2 : filho->info1;

    
    return filho;
}

Arvore23 *buscar_pai(Arvore23 *raiz, int info)
{
    Arvore23 *pai;
    pai = NULL;

    if(raiz != NULL)
    {
        if(!eh_info1(*raiz, info) && !eh_info2(*raiz, info))
        {
            if(info < raiz->info1.numero)
                pai = buscar_pai(raiz->esquerdo, info);
            else if(raiz->n_infos == 1 || info < raiz->info2.numero)
                pai = buscar_pai(raiz->centro, info);
            else
                pai = buscar_pai(raiz->direito, info);

            if(pai == NULL)
                pai = raiz;
        }
    }

    return pai;
}

void movimento_onda(Data saindo, Data *entrada, Arvore23 *pai, Arvore23 **origem, Arvore23 **raiz)
{
    arvore23_remover(raiz, saindo.numero, pai, origem);
    *entrada = saindo;
}

Arvore23 *arvore23_criar()
{
    return NULL;
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

Arvore23 *arvore23_inserir(Arvore23 **raiz, Data info, Arvore23 *pai, Data *promove)
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
            if(info.numero < (*raiz)->info1.numero)
                maior = arvore23_inserir(&((*raiz)->esquerdo), info, *raiz, promove);
            else if((*raiz)->n_infos == 1 || info.numero < (*raiz)->info2.numero)
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

int possivel_remover(Arvore23 *raiz)
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

int arvore23_remover(Arvore23 **raiz, int info, Arvore23 *pai, Arvore23 **origem)
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
                        troca_infos(&((*raiz)->info1), &((*raiz)->info2));

                    (*raiz)->n_infos = 1;
                }
                else
                {
                    if(pai != NULL)
                    {
                        if(*raiz == pai->esquerdo)
                            movimento_onda(pai->info1, &((*raiz)->info1), NULL, origem, origem);
                        else
                        {
                            if(pai->n_infos == 2)
                            {
                                if(*raiz == pai->centro)
                                    movimento_onda(pai->info2, &((*raiz)->info1), NULL, origem, origem);
                                // TODO talvez tenha que fazer ajustes (movimentação estranha na minha cabeça)
                                else
                                    movimento_onda(pai->info2, &(pai->centro->info2), NULL, origem, origem);
                                    // Movimentação "Original"
                                    // movimento_onda(pai->info2, &((*raiz)->info2), origem);
                            }
                            else
                                movimento_onda(pai->info1, &(pai->esquerdo->info2), NULL, origem, origem);
                        }
                    }
                    else
                    {
                        free(*raiz);
                        *raiz = NULL;
                    }
                }
            }
            else
            {
                Arvore23 *pai_aux, *filho;
                pai_aux = *raiz;
                Data info_aux;
                int juntar = 0;

                if(info2)
                {
                    if(possivel_remover((*raiz)->direito))
                        filho = buscar_menor_filho((*raiz)->direito, &pai_aux, &info_aux);
                    else if(possivel_remover((*raiz)->centro))
                        filho = buscar_maior_filho((*raiz)->centro, &pai_aux, &info_aux);
                    // TODO falta fazer (Juntar nó ~Levar em conta o caso de árvore "grande")
                    else
                    {
                        no23_juntar((*raiz)->centro, (*raiz)->direito, &(*raiz)->centro);
                        (*raiz)->n_infos = 1;
                        juntar = 1;
                    }

                    if(!juntar)
                        movimento_onda(info_aux, &((*raiz)->info2), pai_aux, origem, &filho);
                }
                else if(info1)
                {
                    if(possivel_remover((*raiz)->esquerdo))
                        filho = buscar_maior_filho((*raiz)->esquerdo, &pai_aux, &info_aux);
                    else if(possivel_remover((*raiz)->centro))
                        filho = buscar_menor_filho((*raiz)->centro, &pai_aux, &info_aux);
                    else if((*raiz)->n_infos == 1)
                    {
                        if(pai != NULL)
                        {
                            if(*raiz == pai->esquerdo || (pai->n_infos == 2 && (*raiz == pai->centro)))
                            {
                                filho = buscar_menor_filho((*raiz)->centro, &pai_aux, &info_aux);
                                pai_aux = buscar_pai(*origem, pai->info1.numero);

                                if(*raiz == pai->esquerdo)
                                    movimento_onda(pai->info1, &(filho->info2), pai_aux, origem, origem);
                                else
                                    movimento_onda(pai->info2, &(filho->info2), pai_aux, origem, origem);
                            }
                            else
                            {
                                filho = buscar_maior_filho((*raiz)->esquerdo, &pai_aux, &info_aux);
                                pai_aux = buscar_pai(*origem, pai->info1.numero);

                                filho->info2 = filho->info1;
                                if(pai->n_infos == 2 && (*raiz == pai->direito))
                                    movimento_onda(pai->info2, &(filho->info1), pai_aux, origem, origem);
                                else
                                    movimento_onda(pai->info1, &(filho->info1), pai_aux, origem, origem);
                            }
                        }
                        // TODO falta fazer (Juntar nó ~Levar em conta o caso de árvore "grande")
                        else
                        {
                            Arvore23 *aux;
                            aux = *raiz;

                            no23_juntar((*raiz)->esquerdo, (*raiz)->centro, raiz);
                            juntar = 1;
                            
                            no23_desalocar(&aux);
                        }
                    }

                    if(pai != NULL && !juntar)
                        movimento_onda(info_aux, &((*raiz)->info1), pai_aux, origem, &filho);
                }
            }
        }
        else
        {
            if(info < (*raiz)->info1.numero)
                removeu = arvore23_remover(&(*raiz)->esquerdo, info, *raiz, origem);
            else if((*raiz)->n_infos == 1 || info < (*raiz)->info2.numero)
                removeu = arvore23_remover(&(*raiz)->centro, info, *raiz, origem);
            else
                removeu = arvore23_remover(&(*raiz)->direito, info, *raiz, origem);
        }
    }

    return removeu;
}

void arvore23_exibir_pre(Arvore23 *raiz)
{
    if(raiz != NULL)
    {
        printf("[1º] %d -> ", raiz->info1.numero);
        if(raiz->n_infos == 2)
            printf("[2º] %d -> ", raiz->info2.numero);

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
        printf("[1º] %d -> ", raiz->info1.numero);
        arvore23_exibir_ordem(raiz->centro);

        if(raiz->n_infos == 2)
        {
            printf("[2º] %d -> ", raiz->info2.numero);
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

        printf("[1º] %d -> ", raiz->info1.numero);
        if(raiz->n_infos == 2)
            printf("[2º] %d -> ", raiz->info2.numero);
    }
}


int main()
{
    int tam;

    // tam = 10;
    // int valores[] = {5000, 4000, 1000, 2000, 1500, 500, 300, 6000, 8000, 7000};

    // tam = 26;
    // int valores[] = {8000, 10000, 15000, 1000, 3000, 7000, 5800, 4200, 2500, 1800, 9000, 7500, 6500, 4300, 3500, 9500, 2100, 500, 900, 100, 600, 1700, 2400, 1250, 1750, 250};

    tam = 8;
    int valores[] = {10, 60, 50, 30, 150, 100, 80, 200};

    Arvore23 *arvore;
    arvore = arvore23_criar();
    Data *promove = (Data *) malloc(sizeof(Data));

    for(int i = 0; i < tam; i++)
    {
        Data info;
        info.numero = valores[i];
        
        arvore23_inserir(&arvore, info, NULL, promove);
    }
    printf("\n\nÁrvore após inserção: \n");
    arvore23_exibir_pre(arvore);


    // for(int i = tam-1; i >= 0; i--)
    // {
    //     if(valores[i] != 300 && valores[i] != 500 && valores[i] != 1000)
    //     {
    //         arvore23_remover(&arvore, valores[i], NULL, &arvore);
    //         printf("\n\nÁrvore após remover %d:\n", valores[i]);
    //         arvore23_exibir_pre(arvore);
    //     }
    // }

    int valor;
    while(arvore != NULL)
    {
        printf("\nValor a ser removido: ");
        scanf("%d", &valor);
        while(getchar() != '\n');
        arvore23_remover(&arvore, valor, NULL, &arvore);
        printf("\n\nÁrvore após remover %d:\n", valor);
        arvore23_exibir_pre(arvore);
    }

    printf("\n\n");
}