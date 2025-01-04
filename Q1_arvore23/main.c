#include <stdio.h>
#include <stdlib.h>
#include "arquivo/arquivo.h"
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

void leia_str(char *texto, char *variavel)
{
    printf("%s", texto);
    scanf("%[^\n]s", variavel);
    limpa_buffer();
}

// (i) informar uma unidade e então imprima todas as palavras da unidade em português seguida das equivalentes em inglês;
int lista_contem(Lista *lista, int unidade)
{
    int retorno = 0;
    Lista *atual;
    atual = lista;

    while (atual != NULL)
    {
        if (atual->info == unidade)
            retorno = 1;
        atual = atual->proximo;
    }
    return retorno;
}

void exibir_por_unidade_bb(ArvoreBB *arvoreBB, char *palavraPT, int unidade)
{
    if (arvoreBB != NULL)
    {
        exibir_por_unidade_bb(arvoreBB->esquerdo, palavraPT, unidade);

        if (lista_contem(arvoreBB->info.unidade, unidade))
            printf("Palavra em português: %s - Palavra em inglês: %s\n", palavraPT, arvoreBB->info.palavraIngles);
        
        exibir_por_unidade_bb(arvoreBB->direito, palavraPT, unidade);
    }
}

void exibir_por_unidade(Arvore23 *raiz, int unidade)
{
    if(raiz != NULL)
    {
        exibir_por_unidade(raiz->esquerdo, unidade);

        ArvoreBB *arvoreBB;
        arvoreBB = raiz->info1.palavrasEng;

        if(arvoreBB != NULL) 
            exibir_por_unidade_bb(arvoreBB, raiz->info1.palavraPT, unidade);

        exibir_por_unidade(raiz->centro, unidade);
            
        if(raiz->n_infos == 2)
        {
            arvoreBB = raiz->info2.palavrasEng;
            if(arvoreBB != NULL) 
                exibir_por_unidade_bb(arvoreBB, raiz->info2.palavraPT, unidade);
            exibir_por_unidade(raiz->direito, unidade);
        }
    }
}

// (ii)informar uma palavra em português e então imprima todas as palavras em inglês equivalente a palavra em português dada, independente da unidade;
void exibir_traducoes(ArvoreBB *arvoreBB)
{
    if (arvoreBB != NULL)
    {
        exibir_traducoes(arvoreBB->esquerdo);

        printf("- %s\n", arvoreBB->info.palavraIngles);

        exibir_traducoes(arvoreBB->direito);
    }
}
void exibir_traducao_em_ingles(Arvore23 *raiz, char *palavraPT)
{
     Arvore23 *no = arvore23_buscar(raiz, palavraPT);

    if (no != NULL)
    {
        printf("Traduções para '%s':\n", palavraPT);

        ArvoreBB *arvoreBB;

        if(eh_info1(*no, palavraPT))
            arvoreBB = no->info1.palavrasEng;
        else
            arvoreBB = no->info2.palavrasEng;

        if (arvoreBB != NULL)
            exibir_traducoes(arvoreBB);
        else
            printf("Nenhuma tradução encontrada para '%s'.\n", palavraPT);
    }
    else
        printf("Palavra '%s' não encontrada na árvore.\n", palavraPT);
}

// (iii)informar uma palavra em inglês e e a unidade a qual a mesma pertence remove-la das árvores binárias das quais ela pertence. Caso ela seja a única palavra em uma das árvores binárias, remover também da árvore 2-3;

int remover_ingles_unidade_aux_info(DataPT info, char *palavra_ingles, int unidade, char ***palavras_removidas, int *quant_removidas)
{
    int removeu;

    ArvoreBB *no;
    no = arvorebb_buscar(info.palavrasEng, palavra_ingles);

    if(no != NULL)
    {
        int removeu_unidade = lista_remover(&(no->info.unidade), unidade);
        removeu = removeu || removeu_unidade;
        
        if(removeu_unidade && no->info.unidade == NULL)
            arvorebb_remover((&(info.palavrasEng)), palavra_ingles);
    }

    if(info.palavrasEng == NULL)
    {            
        (*quant_removidas)++;
        char **aux;
        aux = (char **) realloc(*palavras_removidas, (*quant_removidas) * sizeof(char **));

        if(aux != NULL)
        {
            *palavras_removidas = aux;
            (*palavras_removidas)[(*quant_removidas) - 1] = info.palavraPT;
        }
    }

    return removeu;
}

int remover_ingles_unidade_aux(Arvore23 **arvore, char *palavra_ingles, int unidade, char ***palavras_removidas, int *quant_removidas)
{
    int removeu = 0;
    
    if(*arvore != NULL)
    {
        removeu = remover_ingles_unidade_aux(&((*arvore)->esquerdo), palavra_ingles, unidade, palavras_removidas, quant_removidas) || removeu;
        removeu = remover_ingles_unidade_aux(&((*arvore)->centro), palavra_ingles, unidade, palavras_removidas, quant_removidas) || removeu;
        if((*arvore)->n_infos == 2)
            removeu = remover_ingles_unidade_aux(&((*arvore)->direito), palavra_ingles, unidade, palavras_removidas, quant_removidas) || removeu;
        
        removeu = remover_ingles_unidade_aux_info((*arvore)->info1, palavra_ingles, unidade, palavras_removidas, quant_removidas) || removeu;
        if((*arvore)->n_infos == 2)
           removeu = remover_ingles_unidade_aux_info((*arvore)->info2, palavra_ingles, unidade, palavras_removidas, quant_removidas) || removeu;
    }

    return removeu;
}
    
int remover_ingles_unidade(Arvore23 **arvore, char *palavra_ingles, int unidade)
{
    char **palavras_removidas;
    palavras_removidas = NULL;
    int quant_removidas = 0;

    int removeu = remover_ingles_unidade_aux(arvore, palavra_ingles, unidade, &palavras_removidas, &quant_removidas);

    for(int i = 0; i < quant_removidas; i++)
        arvore23_remover(arvore, palavras_removidas[i]);

    return removeu;
}

// (iv)informar uma palavra em português e a unidade a qual a mesma pertence e então remove-la, para isto deve remover a palavra em inglês da árvore binária correspondente a palavra em português da mesma unidade. Caso ela seja a única palavra na árvore binária, a palavra em português deve ser removida da árvore 2-3.
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

int remover_portugues_unidade(Arvore23 **arvore, char *palavra_portugues, int unidade)
{
    int removeu = 0;
    Arvore23 *no;
    no = arvore23_buscar(*arvore, palavra_portugues);

    if(no != NULL)
    {
        ArvoreBB **arvoreBB;
        if(eh_info1(*no, palavra_portugues))
            arvoreBB = &no->info1.palavrasEng;
        else
            arvoreBB = &no->info2.palavrasEng;
        
        removeu = remover_portugues_unidade_aux(arvoreBB, unidade);
        if(*arvoreBB == NULL)
            arvore23_remover(arvore, palavra_portugues);
    }

    return removeu;
}


void menu(Arvore23 *arvore)
{
    int op, unidade;
    char *palavra;
    palavra = alocar_str(100);
    do {
        printf("----------[Menu]----------");
        printf("\n[1] - Exibir por unidade");
        printf("\n[2] - Exibir em Inglês");
        printf("\n[3] - Remover Palavra em Inglês de uma Unidade");
        printf("\n[4] - Remover Palavra em Portugues de uma Unidade");
        printf("\n[5] - Exibir Arvore com as Palavras");
        printf("\n[0] - Sair");
        printf("\nOpção: ");
        scanf("%d", &op);
        printf("\n");
        limpa_buffer();

        switch (op)
        {
            case 1:
                leia_int("Digite uma unidade: ", &unidade);
                printf("\n");
                exibir_por_unidade(arvore, unidade);
                break;
            case 2:
                leia_str("Digite uma palavra em português: ", palavra);
                printf("\n");
                exibir_traducao_em_ingles(arvore, palavra);
                break;
            case 3:
                leia_str("Digite uma palavra em inglês: ", palavra);
                leia_int("Digite uma unidade: ", &unidade);
                printf("\n");
                if(remover_ingles_unidade(&arvore, palavra, unidade))
                    printf("[%s] da unidade [%d] removido com sucesso\n", palavra, unidade);
                else
                    printf("[%s] da unidade [%d] não existe\n", palavra, unidade);
                break;
            case 4:
                leia_str("Digite uma palavra em português: ", palavra);
                leia_int("Digite uma unidade: ", &unidade);
                printf("\n");
                if(remover_portugues_unidade(&arvore, palavra, unidade))
                    printf("[%s] da unidade [%d] removido com sucesso\n", palavra, unidade);
                else
                    printf("[%s] da unidade [%d] não existe\n", palavra, unidade);
                break;
            case 5:
                arvore23_exibir_ordem(arvore);
                break;
            case 0: 
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
                break;
        }
        printf("\n");
    } while (op != 0);
}

int main()
{
    char *dicionario = "Dicionario.txt";
    Arvore23 *arvore;
    arvore = arvore23_criar();
    processar_arquivo(dicionario, &arvore);

    menu(arvore);

    arvore23_desalocar(&arvore);
    return 0;
}