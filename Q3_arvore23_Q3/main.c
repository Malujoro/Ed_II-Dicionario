#include <stdio.h>
#include <stdlib.h>

#include "arvore/arvore.h"

void limpa_buffer()
{
    while(getchar() != '\n');
}

void leia_int(char *texto, int *variavel)
{
    printf("%s", texto);
    scanf("%d", variavel);
    limpa_buffer();
}

void leia_numero_no(char *texto, int *variavel, int minimo, int maximo)
{
    do
    {
        leia_int(texto, variavel);
        if(*variavel < minimo || *variavel >= maximo)
            printf("\nDigite um número entre %d e %d\n", minimo, maximo-1);
    } while(*variavel < minimo || *variavel >= maximo);
}

int quantidade_disponivel(Data info)
{
    return info.numero_final - info.numero_inicial + 1;
}

void cadastrar_nos(Arvore23 **arvore, int maximo)
{
    int status;

    do
    {
        printf("\nO primeiro nó está livre ou ocupado?");
        printf("\n[%d] - %s", LIVRE, "Livre");
        printf("\n[%d] - %s", OCUPADO, "Ocupado");
        leia_int("\nStatus: ", &status);

        if(status != LIVRE && status != OCUPADO)
            printf("\nDigite uma opção válida!\n");
    } while(status != LIVRE && status != OCUPADO);

    Data no, promove;
    leia_numero_no("\nEndereço inicial: ", &no.numero_inicial, 0, maximo);

    do
    {
        no.status = status;
        leia_numero_no("\nEndereço final: ", &no.numero_final, no.numero_inicial, maximo);

        arvore23_inserir(arvore, no);

        no.numero_inicial = no.numero_final + 1;
        status = !status;
    }while(no.numero_final < (maximo - 1));
}

Arvore23 *buscar_no_memoria(Arvore23 **arvore, int quant, int status, Data **info_escolhido)
{
    Arvore23 *no;
    if(*arvore != NULL)
    {
        if((*arvore)->info1.status == status && quantidade_disponivel((*arvore)->info1) >= quant)
        {
            *info_escolhido = &((*arvore)->info1);
            no = *arvore;
        }
        else if((*arvore)->n_infos == 2 && (*arvore)->info2.status == status && quantidade_disponivel((*arvore)->info2) >= quant)
        {
            *info_escolhido = &((*arvore)->info2);
            no = *arvore;
        }
        else
        {
            no = buscar_no_memoria(&((*arvore)->esquerdo), quant, status, info_escolhido);
            if(*info_escolhido == NULL)
            {
                no = buscar_no_memoria(&((*arvore)->centro), quant, status, info_escolhido);
                if(*info_escolhido == NULL && (*arvore)->n_infos == 2)
                    no = buscar_no_memoria(&((*arvore)->direito), quant, status, info_escolhido);
            }
        }
    }
    else
        *info_escolhido = NULL;

    return no;
}

void concatenar_no(Arvore23 **raiz, int *numero_final, int limite, int valor_remover)
{
    *numero_final = limite;
    arvore23_remover(raiz, valor_remover);
}

Arvore23 *buscar_menor_bloco(Arvore23 **raiz, Arvore23 no, Data *info, Data **valor_menor)
{
    Arvore23 *menor, *pai;
    *valor_menor = NULL;

    if(eh_folha(no))
    {
        menor = arvore23_buscar_menor_pai(*raiz, info->numero_inicial);
        if(menor != NULL)
        {
            if(menor->n_infos == 2 && menor->info2.numero_inicial < info->numero_inicial)
                *valor_menor = &(menor->info2);
            else
                *valor_menor = &(menor->info1);
        }
    }
    else if(no.info1.numero_inicial == info->numero_inicial)
        menor = arvore23_buscar_maior_filho((*raiz)->esquerdo, &pai, valor_menor);
    else
        menor = arvore23_buscar_maior_filho((*raiz)->centro, &pai, valor_menor);

    return menor;
}

Arvore23 *buscar_maior_bloco(Arvore23 **raiz, Arvore23 no, Data *info, Data **valor_maior)
{
    Arvore23 *maior;
    Arvore23 *pai;
    *valor_maior = NULL;

    if(eh_folha(no))
    {
        maior = arvore23_buscar_maior_pai(*raiz, info->numero_inicial);
        if(maior != NULL)
            *valor_maior = no23_maior_info(maior);
    }
    else
    {
        if(no.info1.numero_inicial == info->numero_inicial)
            maior = arvore23_buscar_menor_filho((*raiz)->centro, &pai);
        else
            maior = arvore23_buscar_menor_filho((*raiz)->direito, &pai);

        if(maior != NULL)
            *valor_maior = &(maior->info1);
    }

    return maior;
}

void modificar_no(Arvore23 **raiz, Arvore23 *no, Data *info, int quant)
{
    Arvore23 *menor;
    Data *valor_menor;

    menor = buscar_menor_bloco(raiz, *no, info, &valor_menor);

    if(quant < quantidade_disponivel(*info))
    {
        if(menor == NULL)
        {
            Data data;
            data.numero_inicial = info->numero_inicial;
            data.numero_final = info->numero_inicial + quant - 1;
            data.status = !(info->status);

            info->numero_inicial += quant;
            arvore23_inserir(raiz, data);
        }
        else
        {
            valor_menor->numero_final += quant;
            info->numero_inicial += quant;
        }
    }
    else
    {
        Arvore23 *maior;
        Data *valor_maior;

        maior = buscar_maior_bloco(raiz, *no, info, &valor_maior);

        if(menor == NULL && maior == NULL)
            info->status = !(info->status);
        else
        {
            if(menor == NULL)
            {
                concatenar_no(raiz, &(info->numero_final), valor_maior->numero_final, valor_maior->numero_inicial);
                info->status = !(info->status);
            }
            else if(maior == NULL)
                concatenar_no(raiz, &(valor_menor->numero_final), info->numero_final, info->numero_inicial);
            else
            {
                int numero = valor_maior->numero_inicial;
                concatenar_no(raiz, &(valor_menor->numero_final), valor_maior->numero_final, info->numero_inicial);
                arvore23_remover(raiz, numero);
            }
        }
    }
}

int main_main()
{
    Arvore23 *arvore;
    arvore = arvore23_criar();

    int maximo = 0;

    do
    {
        leia_int("Quantidade máxima de Blocos de memória: ", &maximo);
        if(maximo <= 0)
            printf("\nDigite uma quantidade positiva\n\n");
    } while(maximo <= 0);

    cadastrar_nos(&arvore, maximo);

    arvore23_exibir_pre(arvore);

    return 0;
}

int main_teste()
{
    Arvore23 *arvore;
    arvore = arvore23_criar();

    // int valores[] = {19, 39, 60, 80, 100};
    int valores[] = {8, 20, 30};
    int tam = sizeof(valores) / sizeof(int);
    int inicio = 0, status = LIVRE;
    Data no;

    for(int i = 0; i < tam; i++)
    {
        no.numero_inicial = inicio;
        no.numero_final = valores[i];
        no.status = status;

        arvore23_inserir(&arvore, no);

        status = !status;
        inicio = no.numero_final + 1;
    }

    arvore23_exibir_ordem(arvore);

    int quant = 10;
    status = LIVRE;
    // status = OCUPADO;

    Data *info_escolhido;
    info_escolhido = NULL;
    Arvore23 *no_escolhido;
    no_escolhido = buscar_no_memoria(&arvore, quant, status, &info_escolhido);

    if(info_escolhido != NULL)
    {
        printf("\nNó escolhido: \n");
        no23_exibir(*info_escolhido);
        printf("\n");

        modificar_no(&arvore, no_escolhido, info_escolhido, quant);
        
        arvore23_exibir_ordem(arvore);
    }
    else
        printf("\nNão há nó disponível\n");
}

int main()
{
    // main_main();
    main_teste();
}