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

Arvore23 *no23_juntar(Arvore23 *filho1, Data info, Arvore23 *maior, Arvore23 **raiz)
{
    if(filho1->n_infos == 2)
        filho1->n_infos = 1;
        
    no23_adicionar_info(filho1, info, maior);

    (*raiz)->n_infos--;

    if((*raiz)->n_infos == 0)
        no23_desalocar(raiz);

    return filho1;
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

Arvore23 *buscar_menor_filho(Arvore23 *raiz, Arvore23 **pai)
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

Data maior_info(Arvore23 *raiz)
{
    return raiz->n_infos == 2 ? raiz->info2 : raiz->info1;
}

Arvore23 *buscar_maior_filho(Arvore23 *raiz, Arvore23 **pai, Data *maior_valor)
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
        *maior_valor = maior_info(filho);

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

Arvore23 *buscar_maior_pai(Arvore23 *origem, Arvore23 *pai, int info, Data *maior)
{
    while(pai != NULL && ((pai->n_infos == 1 && pai->info1.numero < info) || (pai->n_infos == 2 && pai->info2.numero < info)))
        pai = buscar_pai(origem, pai->info1.numero);

    if(pai != NULL)
        *maior = maior_info(pai);

    return pai;
}

int movimento_onda(Data saindo, Data *entrada, Arvore23 *pai, Arvore23 **origem, Arvore23 **raiz, Arvore23 **maior)
{
    int removeu = arvore23_remover1(raiz, saindo.numero, pai, origem, maior);
    *entrada = saindo;
    return removeu;
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

int arvore23_remover1(Arvore23 **raiz, int info, Arvore23 *pai, Arvore23 **origem, Arvore23 **maior)
{
    int removeu = 0;

    if(*raiz != NULL)
    {
        Arvore23 *pai_aux;
        pai_aux = pai;
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
                        
                        if(*raiz == pai->esquerdo || (pai->n_infos == 2 && *raiz == pai->centro))
                        {
                            pai_aux = buscar_pai(*origem, pai->info1.numero);
                            
                            if(*raiz == pai->esquerdo)
                                removeu = movimento_onda(pai->info1, &((*raiz)->info1), pai_aux, origem, origem, maior);
                            else 
                                removeu = movimento_onda(pai->info2, &((*raiz)->info1), pai_aux, origem, origem, maior);
                        }
                        else
                        {
                            Data info_pai;
                            pai_aux = buscar_maior_pai(*origem, pai, (*raiz)->info1.numero, &info_pai);
                            if(pai_aux != NULL)
                            {
                                Arvore23 *avo;
                                avo = buscar_pai(*origem, info_pai.numero);
                                removeu = movimento_onda(info_pai, &((*raiz)->info1), avo, origem, origem, maior);
                            }
                            else
                            {
                                (*raiz)->n_infos = 0;
                                removeu = -1;
                                // TODO desalocar verificando
                                // no23_desalocar(raiz);
                            }
                        }
                    }
                    else
                        no23_desalocar(raiz);
                }
            }
            else
            {
                Arvore23 *filho, *filho2;
                Arvore23 *pai_aux2;
                filho2 = NULL;
                Data info_filho;

                if(info2)
                {
                    filho = buscar_menor_filho((*raiz)->direito, &pai_aux);

                    if(filho->n_infos == 1)
                        filho2 = buscar_maior_filho((*raiz)->centro, &pai_aux2, &info_filho);
                    
                    if(filho2 != NULL && filho2->n_infos == 2)
                    {
                        (*raiz)->info2 = filho2->info2;
                        filho2->n_infos = 1;
                    }
                    else
                    {
                        if(pai_aux == NULL)
                            pai_aux = *raiz;
                        removeu = movimento_onda(filho->info1, &((*raiz)->info2), pai_aux, origem, &filho, maior);
                    }
                }
                else if(info1)
                {
                    filho2 = buscar_maior_filho((*raiz)->esquerdo, &pai_aux, &info_filho);

                    if(filho2->n_infos == 2)
                    {
                        (*raiz)->info1 = info_filho;
                        filho2->n_infos = 1;
                    }
                    else
                    {
                        filho = buscar_menor_filho((*raiz)->centro, &pai_aux);

                        if(pai_aux == NULL)
                            pai_aux = *raiz;
                        removeu = movimento_onda(filho->info1, &((*raiz)->info1), pai_aux, origem, &filho, maior);
                    }
                }
            }
        }
        else
        {
            if(info < (*raiz)->info1.numero)
                removeu = arvore23_remover1(&(*raiz)->esquerdo, info, *raiz, origem, maior);
            else
            {
                if((*raiz)->n_infos == 1 || info < (*raiz)->info2.numero)
                    removeu = arvore23_remover1(&(*raiz)->centro, info, *raiz, origem, maior);
                else
                    removeu = arvore23_remover1(&(*raiz)->direito, info, *raiz, origem, maior);
            }
        }
    }

    // if(*raiz != NULL && !eh_folha(**raiz))
    // {
    //     if((*raiz)->n_infos == 1 && ((*raiz)->centro == NULL || (*raiz)->centro->n_infos == 0))
    //     {
    //         if((*raiz)->centro != NULL)
    //             no23_desalocar(&((*raiz)->centro));
    //         *maior = no23_juntar((*raiz)->esquerdo, (*raiz)->info1, *maior, raiz);
    //     }
    //     else if((*raiz)->n_infos == 2 && ((*raiz)->direito == NULL || (*raiz)->direito->n_infos == 0))
    //     {
    //         if((*raiz)->direito != NULL)
    //             no23_desalocar(&((*raiz)->direito));
    //         *maior = no23_juntar((*raiz)->centro, (*raiz)->info2, *maior, raiz);
    //     }
    // }

    // if(*origem == NULL)
    //     *origem = *maior;

    return removeu;
}

// TODO verificar função de balancear
/* TODO
    Criar uma função que vai gerenciar a remoção
    Vai remover normal, retornando um valor indicando se precisa de balanceamento
    Caso precise de balanceamento e seja possível remover, executa uma "remoção reversa" para ocupar o espaço que tá Nulo (com n_infos = 0) 
*/

int arvore23_rebalancear(Arvore23 **raiz, Arvore23 **maior)
{
    int balanceou = 0;
    if(*raiz != NULL)
    {
        if(!eh_folha(**raiz))
        {
            if((*raiz)->n_infos == 1)
            {
                balanceou = arvore23_rebalancear(&((*raiz)->centro), maior);
                if((*raiz)->centro == NULL || (*raiz)->centro->n_infos == 0)
                {
                    if((*raiz)->centro != NULL)
                        no23_desalocar(&((*raiz)->centro));

                    *maior = no23_juntar((*raiz)->esquerdo, (*raiz)->info1, *maior, raiz);
                    balanceou = 1;
                }
            }
            else if((*raiz)->n_infos == 2)
            {
                balanceou = arvore23_rebalancear(&((*raiz)->direito), maior);
                if((*raiz)->direito == NULL || (*raiz)->direito->n_infos == 0)
                {
                    if((*raiz)->direito != NULL)
                        no23_desalocar(&((*raiz)->direito));

                    *maior = no23_juntar((*raiz)->centro, (*raiz)->info2, *maior, raiz);
                    balanceou = 1;
                }
            }
        }
    }

    return balanceou;
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


int arvore23_remover(Arvore23 **raiz, int info)
{   
    Arvore23 *maior;
    maior = NULL;
    int removeu = arvore23_remover1(raiz, info, NULL, raiz, &maior);

    if(removeu == -1)
    {
        removeu = arvore23_rebalancear(raiz, &maior);
        if(*raiz == NULL)
            *raiz = maior;
    }
    
    // if(removeu)

    return removeu;
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
    // int valores[] = {8000, 10000, 15000, 1000, 3000, 7000, 5800, 4200, 2500, 1800, 9000, 7500, 6500, 4300, 3500, 9500, 2100, 500, 900, 100, 600, 1700, 2400, 1250, 1750, 250};
    // int valores[] = {30, 120, 100, 50, 170, 150, 140, 200};
    int valores[] = {2, 4, 7, 3, 1, 6, 5};
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

    // for(int i = tam-1; i >= 0; i--)
    for(int i = 0; i >= 0; i--)
    {
        arvore23_remover(&arvore, valores[i]);
        // arvore23_remover1(&arvore, valores[i], NULL, &arvore, &maior);
        printf("\n\nÁrvore após remover %d:\n", valores[i]);
        arvore23_exibir_pre(arvore);
    }

    // int valor;
    // while(arvore != NULL)
    // {
    //     printf("\nValor a ser removido: ");
    //     scanf("%d", &valor);
    //     while(getchar() != '\n');
    //     arvore23_remover1(&arvore, valor, NULL, &arvore);
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

        arvore23_remover(&arvore, valor);
        // arvore23_remover1(&arvore, valor, NULL, &arvore, &maior);

        printf("\n\nÁrvore após remover %d:\n", valor);
        arvore23_exibir_pre(arvore);
        printf("\n\n");

        arvore23_desalocar(&arvore);
    }
    return 0;
}

int main()
{
    main1();
    // main2();

    return 0;
}