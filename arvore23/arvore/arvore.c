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

        Arvore23 *pai, *filho;
        Data aux;
        filho = buscar_menor_filho(maior, &pai, &aux);

        arvore23_inserir(&maior, filho2->info1, NULL, &aux);
        movimento_onda(filho->info1, &(filho2->info1), pai, &maior, &maior);

        // Data aux = filho2->info1;
        // filho2->info1 = maior->info1;
        // maior->info1 = aux;
    }

    filho1->info2 = filho2->info1;
    filho1->n_infos = 2;
    maior = filho1;
    *filho3 = maior;
    filho2->n_infos = 0;
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
        Arvore23 *pai_aux;
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
                        pai_aux = buscar_pai(*origem, pai->info1.numero);

                        if(*raiz == pai->esquerdo)
                            movimento_onda(pai->info1, &((*raiz)->info1), pai_aux, origem, origem);
                        else
                        {
                            if(pai->n_infos == 2)
                            {
                                if(*raiz == pai->centro)
                                    movimento_onda(pai->info2, &((*raiz)->info1), pai_aux, origem, origem);
                                else
                                    movimento_onda(pai->info2, &(pai->centro->info2), pai_aux, origem, origem);
                            }
                            // TODO talvez esteja certo
                            else
                            {
                                if(pai_aux != NULL)
                                {

                                    if(pai_aux->n_infos == 1)
                                    {
                                        while(pai_aux != NULL && pai_aux->info1.numero < (*raiz)->info1.numero)
                                            pai_aux = buscar_pai(*origem, pai_aux->info1.numero);
                                    }
                                    else if(pai_aux->n_infos == 2)
                                    {
                                        while(pai_aux != NULL && pai_aux->n_infos == 2 && pai_aux->info2.numero < (*raiz)->info1.numero)
                                            pai_aux = buscar_pai(*origem, pai_aux->info1.numero);
                                    }

                                }
                                
                                Arvore23 *avo;
                                if(pai_aux == NULL)
                                    movimento_onda(pai->info1, &(pai->esquerdo->info2), pai_aux, origem, origem);
                                else
                                {
                                    avo = buscar_pai(*origem, pai_aux->info1.numero);
                                    if(pai_aux->n_infos == 1)
                                        movimento_onda(pai_aux->info1, &((*raiz)->info1), avo, origem, origem);
                                    else
                                        movimento_onda(pai_aux->info2, &((*raiz)->info1), avo, origem, origem);
                                }
                            }
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
                Arvore23 *filho;
                pai_aux = *raiz;
                Data info_aux;
                int juntar = 0;

                if(info2)
                {
                    if(possivel_remover((*raiz)->direito))
                        filho = buscar_menor_filho((*raiz)->direito, &pai_aux, &info_aux);
                    else if(possivel_remover((*raiz)->centro))
                        filho = buscar_maior_filho((*raiz)->centro, &pai_aux, &info_aux);
                    else
                    {
                        filho = no23_juntar((*raiz)->centro, (*raiz)->direito, &(*raiz)->centro);
                        (*raiz)->n_infos = 1;
                        juntar = 1;
                    }

                    if(!juntar && (*raiz)->n_infos != 0)
                        movimento_onda(info_aux, &((*raiz)->info2), pai_aux, origem, &filho);
                }
                else if(info1)
                {
                    if(possivel_remover((*raiz)->esquerdo))
                        filho = buscar_maior_filho((*raiz)->esquerdo, &pai_aux, &info_aux);
                    else if((*raiz)->n_infos == 2 || possivel_remover((*raiz)->centro))
                        filho = buscar_menor_filho((*raiz)->centro, &pai_aux, &info_aux);
                    else if((*raiz)->n_infos == 1)
                    {
                        if(pai != NULL)
                        {
                            if(*raiz == pai->esquerdo || (pai->n_infos == 2 && (*raiz == pai->centro)))
                            {
                                filho = buscar_maior_filho((*raiz)->centro, &pai_aux, &info_aux);
                                pai_aux = buscar_pai(*origem, pai->info1.numero);

                                if(*raiz == pai->esquerdo)
                                    movimento_onda(pai->info1, &(filho->info2), pai_aux, origem, origem);
                                else
                                    movimento_onda(pai->info2, &(filho->info2), pai_aux, origem, origem);
                                filho->n_infos = 2;

                                filho = buscar_menor_filho((*raiz)->centro, &pai_aux, &info_aux);
                            }
                            else if((*raiz)->info1.numero > (*origem)->info1.numero)
                            {
                                filho = buscar_menor_filho((*raiz)->esquerdo, &pai_aux, &info_aux);
                                pai_aux = buscar_pai(*origem, pai->info1.numero);

                                filho->info2 = filho->info1;

                                if((pai->n_infos == 2 && possivel_remover(pai->centro)) || (pai->n_infos == 1 && possivel_remover(pai->esquerdo)))
                                {
                                    if(pai->n_infos == 2 && (*raiz == pai->direito))
                                        movimento_onda(pai->info2, &(filho->info1), pai_aux, origem, origem);
                                    else
                                        movimento_onda(pai->info1, &(filho->info1), pai_aux, origem, origem);
                                }
                                else
                                {
                                    if(pai->n_infos == 2 && (*raiz == pai->direito))
                                        movimento_onda(pai->info2, &(pai->centro->info2), pai_aux, origem, origem);
                                    else
                                        movimento_onda(pai->info1, &(pai->centro->info2), pai_aux, origem, origem);
                                }

                                filho->n_infos = 2;

                                filho = buscar_maior_filho((*raiz)->esquerdo, &pai_aux, &info_aux);
                            }
                            else
                                filho = buscar_maior_filho((*raiz)->centro, &pai_aux, &info_aux);
                        }
                        else
                        {
                            Arvore23 *aux;
                            aux = *raiz;


                            no23_juntar((*raiz)->esquerdo, (*raiz)->centro, raiz);
                            juntar = 1;
                            
                            no23_desalocar(&aux);
                        }
                    }

                    if(pai_aux == NULL)
                        pai_aux = pai;

                    if(!juntar && (*raiz)->n_infos != 0)
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
        // int valores[] = {50, 150, 100, 200, 300, 350, 400, 500, 600, 450};
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
    // int valores[] = {30, 120, 100, 50, 170, 150, 140, 200};
    tam = sizeof(valores) / sizeof(int);

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


    for(int i = tam-1; i >= 0; i--)
    {
        arvore23_remover(&arvore, valores[i], NULL, &arvore);
        printf("\n\nÁrvore após remover %d:\n", valores[i]);
        arvore23_exibir_pre(arvore);
    }

    // int valor;
    // while(arvore != NULL)
    // {
    //     printf("\nValor a ser removido: ");
    //     scanf("%d", &valor);
    //     while(getchar() != '\n');
    //     arvore23_remover(&arvore, valor, NULL, &arvore);
    //     printf("\n\nÁrvore após remover %d:\n", valor);
    //     arvore23_exibir_pre(arvore);
    // }

    printf("\n\n");
    return 0;
}

int main2()
{
    int tam, valor = 100;
    int *valores;

    Arvore23 *arvore;
    arvore = arvore23_criar();
    Data promove;

    for(int i = 1; i <= 19; i++)
    {
        printf("\n\n[%dº caso]", i);

        valores = caso_selecionado(i, &tam);
        for(int j = 0; j < tam; j++)
        {
            Data info;
            info.numero = valores[j];
            arvore23_inserir(&arvore, info, NULL, &promove);
        }
        printf("\n\nÁrvore após inserção: \n");
        arvore23_exibir_pre(arvore);

        arvore23_remover(&arvore, valor, NULL, &arvore);

        printf("\n\nÁrvore após remover %d:\n", valor);
        arvore23_exibir_pre(arvore);
        printf("\n\n");

        arvore23_desalocar(&arvore);
    }
    return 0;
}

int main()
{
    // main1();
    main2();

    return 0;
}