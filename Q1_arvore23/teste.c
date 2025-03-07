#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "arvore/arvore.h"
#include "arquivo/arquivo.h"

typedef double tempo_tipo;

tempo_tipo calcula_tempo(clock_t inicio, clock_t fim)
{
    return ((tempo_tipo) (fim - inicio)) / CLOCKS_PER_SEC * 1000 * 1000;
}

DataPT preencher_no(char *palavra)
{
    DataPT info;

    info.palavraPT = alocar_str(50);
    strcpy(info.palavraPT, palavra);
    info.palavrasEng = arvorebb_criar();
    
    return info;
}

Arvore23 *montar_arvore(int quant, int tam)
{
    Arvore23 *arvore;
    arvore = arvore23_criar();

    int atual = 0;
    char *palavra = alocar_str(tam);
    for(int i = 0; i < tam; i++)
        palavra[i] = 'a';

    do
    {
        for(int i = tam-1; i >= 0; i--)
        {
            for(int j = 'a'; j <= 'z' && atual < quant; j++)
            {
                palavra[i] = j;
                DataPT info = preencher_no(palavra);
                arvore23_inserir(&arvore, info);
                atual++;
            }
        }

    } while(atual < quant);

    return arvore;
}

tempo_tipo calcular_tempo_medio(Arvore23 **arvore, char *info, int repeticoes)
{
    tempo_tipo media = 0;

    for(int i = 0; i < repeticoes; i++)
    {
        clock_t inicio, fim;
        Arvore23 *no_encontrado;
        tempo_tipo tempo_gasto;

        inicio = clock();

        no_encontrado = arvore23_buscar(*arvore, info);

        fim = clock();
        
        tempo_gasto = calcula_tempo(inicio, fim);
        // if(no_encontrado != NULL)
        //     printf("Tempo médio de execução: %lf microssegundos\n", tempo_gasto);
        media += tempo_gasto;
    }
    
    media /= repeticoes;
    return media;
}

Arvore23 *arvore23_buscar_caminho(Arvore23 *raiz, char *info)
{
    Arvore23 *no;
    no = NULL;

    if(raiz != NULL)
    {
        printf("[1º] %s ", raiz->info1.palavraPT);

        if(raiz->n_infos == 2)
            printf("| [2º] %s ", raiz->info2.palavraPT);
        
        printf("-> ");

        if(eh_info1(*raiz, info) || eh_info2(*raiz, info))
            no = raiz;
        else if(strcmp(info, raiz->info1.palavraPT) < 0)
            no = arvore23_buscar_caminho(raiz->esquerdo, info);
        else if(raiz->n_infos == 1 || (strcmp(info, raiz->info2.palavraPT) < 0))
            no = arvore23_buscar_caminho(raiz->centro, info);
        else
            no = arvore23_buscar_caminho(raiz->direito, info);
    }

    return no;
}

int main()
{
    srand(1);

    int tamanho = 5, repeticoes = 30;
    int quant_nos = pow(26, tamanho);

    char palavras[][50] =  {"amor", "biscoito", "carro", "dado", "elefante", "futebol", "gato", "homem", "igreja", "jacaré", "kiwi", "laranja", "montanha", "navio", "ovo", "pato", "queijo", "rato", "sapato", "tigre", "uva", "vento", "wesley", "xadrez", "yasmin", "zebra", "porta", "jardim", "livro", "telefone"};

    int quant_palavras = sizeof(palavras) / sizeof(char [50]);

    Arvore23 *arvore;
    arvore = montar_arvore(quant_nos, tamanho);
    
    arvore23_exibir_ordem(arvore);

    for(int i = 0; i < quant_palavras; i++)
    {
        DataPT info = preencher_no(palavras[i]);
        arvore23_inserir(&arvore, info);
    }

    for(int i = 0; i < quant_palavras; i++)
    {
        // arvore23_buscar_caminho(arvore, palavras[i]);
        // printf("\n");
        tempo_tipo media = calcular_tempo_medio(&arvore, palavras[i], repeticoes);
        printf("[%dº] [%s] Tempo médio de execução: %lf microssegundos\n\n", i+1, palavras[i], media);
    }
    printf("\n[Árvore de %d elementos]\n", quant_nos + quant_palavras);
    arvore23_desalocar(&arvore);

    return 0;
}