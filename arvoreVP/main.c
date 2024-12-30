#include <stdio.h>
#include <stdlib.h>
#include "arquivo/arquivo.h"
#include "arvore/arvore.h"

// (i) informar uma unidade e então imprima todas as palavras da unidade em português seguida das equivalentes em inglês;
// void exibir_por_unidade()

// (ii)informar uma palavra em português e então imprima todas as palavras em inglês equivalente a palavra em português dada, independente da unidade;
// void exibir_traducao_em_ingles()

// (iii)informar uma palavra em inglês e e a unidade a qual a mesma pertence remove-la das árvores binárias das quais ela pertence. Caso ela seja a única palavra em uma das árvores binárias, remover também da árvore 2-3;
int remover_ingles_unidade(ArvoreVP **arvore, char *palavra_ingles, int unidade)
{
    // TODO Dado uma palavra em inglês e uma unidade, remover todas as palavras inglesas daquela unidade (em toda a árvore português). Caso a árvore inglês fique nula, remover aquela palavra português também
    // Criar uma auxiliar pra fazer essas remoções, e uma outra auxiliar pra verificar em toda a árvore português se há alguma palavra sem tradução pra inglẽs e então removê-la
    int removeu = 0;
    ArvoreBB *no;
    
    if(*arvore != NULL)
    {
        removeu = remover_ingles_unidade(&((*arvore)->esquerdo), palavra_ingles, unidade) || removeu;
        removeu = remover_ingles_unidade(&((*arvore)->direito), palavra_ingles, unidade) || removeu;

        no = arvorebb_buscar((*arvore)->info.palavrasEng, palavra_ingles);

        if(no != NULL)
        {
            removeu = lista_remover(&(no->info.unidade), unidade) || removeu;
            if(removeu && no->info.unidade == NULL)
                arvorebb_remover((&(*arvore)->info.palavrasEng), palavra_ingles);
        }


        // if((*arvore)->info.palavrasEng == NULL)
        //     arvorevp_remover(arvore, (*arvore)->info.palavraPT);
    }

    return removeu;
}

// (iv)informar uma palavra em português e a unidade a qual a mesma pertence e então remove-la, para isto deve remover a palavra em inglês da árvore binária correspondente a palavra em português da mesma unidade. Caso ela seja a única palavra na árvore binária, a palavra em português deve ser removida da árvore 2-3.

// TODO Dado uma palavra em português e uma unidade, remover todas as palavras inglesas daquela unidade. Caso não haja mais palavras (árvore vazia), remover a própria palavra em português
// Criar uma auxiliar pra fazer as remoções, e a "main" faz a verificação e remoção da própria palavra

// int remover_portugues_unidade(ArvoreVP **arvore, char *palavra_portugues, int unidade)


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

    char *palavra = {"Bug"};
    int unidade = 2;
    printf("\n[Removendo %s da Unidade %d]\n", palavra, unidade);
    remover_ingles_unidade(&arvore, palavra, unidade);
    remover_ingles_unidade(&arvore, palavra, 1);
    printf("\n\n");
    arvorevp_exibir_pre(arvore);
    return 0;
}