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

ArvoreVP *montar_arvore(int quant, int tam)
{
    ArvoreVP *arvore;
    arvore = arvorevp_criar();

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
                arvorevp_inserir(&arvore, info);
                atual++;
            }
        }

    } while(atual < quant);

    return arvore;
}

tempo_tipo calcular_tempo_medio(ArvoreVP **arvore, char *info, int repeticoes)
{
    tempo_tipo media = 0;

    for(int i = 0; i < repeticoes; i++)
    {
        clock_t inicio, fim;
        ArvoreVP *no_encontrado;
        tempo_tipo tempo_gasto;

        inicio = clock();

        no_encontrado = arvorevp_buscar(*arvore, info);

        fim = clock();
        
        tempo_gasto = calcula_tempo(inicio, fim);
        // if(no_encontrado != NULL)
        //     printf("Tempo médio de execução: %lf microssegundos\n", tempo_gasto);
        media += tempo_gasto;
    }
    
    media /= repeticoes;
    return media;
}

ArvoreVP *arvorevp_buscar_caminho(ArvoreVP *raiz, char *palavra)
{
    ArvoreVP *retorno;

    if(raiz != NULL)
    {
        printf("%s -> ", raiz->info.palavraPT);

        int resultado = strcmp(palavra, raiz->info.palavraPT);

        if(resultado == 0)
            retorno = raiz;
        else if(resultado < 0)
            retorno = arvorevp_buscar_caminho(raiz->esquerdo, palavra);
        else if(resultado > 0)
            retorno = arvorevp_buscar_caminho(raiz->direito, palavra);
    }
    else
        retorno = NULL;
    
    return retorno;
}

int main()
{
    srand(1);

    int tamanho = 5, repeticoes = 30;
    int quant_nos = pow(26, tamanho);

    char palavras[][50] =  {"amor", "biscoito", "carro", "dado", "elefante", "futebol", "gato", "homem", "igreja", "jacaré", "kiwi", "laranja", "montanha", "navio", "ovo", "pato", "queijo", "rato", "sapato", "tigre", "uva", "vento", "wesley", "xadrez", "yasmin", "zebra", "porta", "jardim", "livro", "telefone"};

    int quant_palavras = sizeof(palavras) / sizeof(char [50]);

    ArvoreVP *arvore;
    arvore = montar_arvore(quant_nos, tamanho);
    
    arvorevp_exibir_ordem(arvore);

    for(int i = 0; i < quant_palavras; i++)
    {
        DataPT info = preencher_no(palavras[i]);
        arvorevp_inserir(&arvore, info);
    }
    for(int i = 0; i < quant_palavras; i++)
    {
        // arvorevp_buscar_caminho(arvore, palavras[i]);
        tempo_tipo media = calcular_tempo_medio(&arvore, palavras[i], repeticoes);
        printf("[%dº] [%s] Tempo médio de execução: %lf microssegundos\n\n", i+1, palavras[i], media);

    }
    printf("\n[Árvore de %d elementos]\n", quant_nos + quant_palavras);
    arvorevp_desalocar(&arvore);

    return 0;
}