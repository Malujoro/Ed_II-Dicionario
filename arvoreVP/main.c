#include "arquivo/arquivo.h"
#include "arvore/arvore.h"

// (i) informar uma unidade e então imprima todas as palavras da unidade em português seguida das equivalentes em inglês;
// void exibir_por_unidade()

// (ii)informar uma palavra em português e então imprima todas as palavras em inglês equivalente a palavra em português dada, independente da unidade;
// void exibir_traducao_em_ingles()

// (iii)informar uma palavra em inglês e e a unidade a qual a mesma pertence remove-la das árvores binárias das quais ela pertence. Caso ela seja a única palavra em uma das árvores binárias, remover também da árvore 2-3;
// void remover_ingles_unidade()

// (iv)informar uma palavra em português e a unidade a qual a mesma pertence e então remove-la, para isto deve remover a palavra em inglês da árvore binária correspondente a palavra em português da mesma unidade. Caso ela seja a única palavra na árvore binária, a palavra em português deve ser removida da árvore 2-3.
// void remover_portugues_unidade()


void menu()
{
    
}

int main()
{
    char *dicionario = "Dicionario.txt";
    ArvoreVP *arvore;
    arvore = arvorevp_criar();
    processar_arquivo(dicionario, &arvore);

    arvorevp_exibir_pre(arvore);
    return 0;
}