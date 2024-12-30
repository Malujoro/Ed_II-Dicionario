#include <stdio.h>
#include <stdlib.h>
#include "arquivo/arquivo.h"
#include "arvore/arvore.h"

// (i) informar uma unidade e então imprima todas as palavras da unidade em português seguida das equivalentes em inglês;
// void exibir_por_unidade()

// (ii)informar uma palavra em português e então imprima todas as palavras em inglês equivalente a palavra em português dada, independente da unidade;
// void exibir_traducao_em_ingles()

int remover_ingles_unidade_aux(ArvoreVP **arvore, char *palavra_ingles, int unidade, char ***palavras_removidas, int *quant_removidas)
{
    int removeu = 0;
    ArvoreBB *no;
    
    if(*arvore != NULL)
    {
        removeu = remover_ingles_unidade_aux(&((*arvore)->esquerdo), palavra_ingles, unidade, palavras_removidas, quant_removidas) || removeu;
        removeu = remover_ingles_unidade_aux(&((*arvore)->direito), palavra_ingles, unidade, palavras_removidas, quant_removidas) || removeu;
        
        no = arvorebb_buscar((*arvore)->info.palavrasEng, palavra_ingles);

        if(no != NULL)
        {
            int removeu_unidade = lista_remover(&(no->info.unidade), unidade);
            removeu = removeu || removeu_unidade;
            
            if(removeu_unidade && no->info.unidade == NULL)
                arvorebb_remover((&(*arvore)->info.palavrasEng), palavra_ingles);
        }

        if((*arvore)->info.palavrasEng == NULL)
        {            
            (*quant_removidas)++;
            char **aux;
            aux = (char **) realloc(*palavras_removidas, (*quant_removidas) * sizeof(char **));

            if(aux != NULL)
            {
                *palavras_removidas = aux;
                (*palavras_removidas)[(*quant_removidas) - 1] = (*arvore)->info.palavraPT;
            }
        }
    }

    return removeu;

}
    
int remover_ingles_unidade(ArvoreVP **arvore, char *palavra_ingles, int unidade)
{
    char **palavras_removidas;
    palavras_removidas = NULL;
    int quant_removidas = 0;

    int removeu = remover_ingles_unidade_aux(arvore, palavra_ingles, unidade, &palavras_removidas, &quant_removidas);

    for(int i = 0; i < quant_removidas; i++)
        arvorevp_remover(arvore, palavras_removidas[i]);

    return removeu;
}

int remover_portugues_unidade_aux(ArvoreBB **arvore, int unidade)
{
    int removeu = 0;

    if(*arvore != NULL)
    {
        removeu = remover_portugues_unidade_aux(&((*arvore)->direito), unidade) || removeu;
        removeu = remover_portugues_unidade_aux(&((*arvore)->esquerdo), unidade) || removeu;

        int removeu_unidade = lista_remover(&((*arvore)->info.unidade), unidade);
        removeu = removeu || removeu_unidade;
        if(removeu_unidade && (*arvore)->info.unidade == NULL)
            arvorebb_remover(arvore, (*arvore)->info.palavraIngles);
    }

    return removeu;
}

int remover_portugues_unidade(ArvoreVP **arvore, char *palavra_portugues, int unidade)
{
    int removeu = 0;
    ArvoreVP *no;
    no = arvorevp_buscar(*arvore, palavra_portugues);

    if(no != NULL)
    {
        removeu = remover_portugues_unidade_aux(&(no->info.palavrasEng), unidade);
        if(no->info.palavrasEng == NULL)
            arvorevp_remover(arvore, palavra_portugues);
    }

    return removeu;
}


void menu()
{
    
}

int main()
{
    char *dicionario = "Dicionario.txt";
    ArvoreVP *arvore;
    arvore = arvorevp_criar();
    processar_arquivo(dicionario, &arvore);

    // char *palavra = {"Bug"};
    // int unidade = 2;
    // printf("\n[Removendo %s da Unidade %d]\n", palavra, unidade);
    // remover_ingles_unidade(&arvore, palavra, unidade);
    // remover_ingles_unidade(&arvore, palavra, 1);
    // printf("\n\n");
    // arvorevp_exibir_pre(arvore);


    // char *palavra = {"Fan"};
    // int unidade = 2;
    // printf("\n[Removendo %s da Unidade %d]\n", palavra, unidade);
    // remover_ingles_unidade(&arvore, "Coller", 1);
    // remover_ingles_unidade(&arvore, palavra, unidade);
    // printf("\n\n");
    // arvorevp_exibir_pre(arvore);


    // char *palavra = {"ventilador"};
    // int unidade = 1;
    // printf("\n[Removendo %s da Unidade %d]\n", palavra, unidade);
    // remover_portugues_unidade(&arvore, palavra, unidade);
    // remover_portugues_unidade(&arvore, palavra, 2);
    // printf("\n\n");
    // arvorevp_exibir_pre(arvore);
    return 0;
}