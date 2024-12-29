#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../arvore/arvore.h"

char *alocar_str(int tam)
{
    char *vetor;
    vetor = (char *) malloc(sizeof(char) * tam);

    if(!vetor)
    {
        printf("Erro ao alocar vetor de char");
        exit(EXIT_FAILURE);
    }

    return vetor;
}

FILE* abrir_arquivo(const char *nome_arquivo) {
    FILE *arquivo;
    arquivo = fopen(nome_arquivo, "r");

    if (!arquivo) {
        printf("Erro ao abrir o arquivo %s!\n", nome_arquivo);
        exit(EXIT_FAILURE);
    }

    return arquivo;
}

void identificar_unidade(const char *linha, int *unidade) {
    char *aux;
    aux = alocar_str(50);
    if (linha[0] == '%') {
        strcpy(aux, linha + 10);
        sscanf(aux, "%d", unidade);
    }
}

int identificar_traducoes(const char *linha, char *palavra_ingles, char traducoes[][50], int *num_traducoes) {
    int retorno = 0;

    if (strchr(linha, ':')) {
        char buffer_traducoes[256];
        sscanf(linha, "%[^:]: %[^\n]", palavra_ingles, buffer_traducoes);

        char *traducao;
        traducao = strtok(buffer_traducoes, ",;");
        *num_traducoes = 0;

        while (traducao) {
            if(traducao[0] == ' ')
                traducao++;

            strcpy(traducoes[*num_traducoes], traducao);
            (*num_traducoes)++;
            traducao = strtok(NULL, ",;");
        }
        retorno = 1;
    }
    return retorno; 
}

void processar_arquivo(const char *nome_arquivo, ArvoreVP **arvore) {
    FILE *arquivo;
    arquivo = abrir_arquivo(nome_arquivo);
    if (arquivo)
    {
        char linha[256];
        int unidade_atual;

        while (fgets(linha, sizeof(linha), arquivo)) {
            linha[strcspn(linha, "\n")] = '\0';

            identificar_unidade(linha, &unidade_atual);

            char *palavra_ingles;
            palavra_ingles = alocar_str(50);
            char traducoes[10][50];
            int num_traducoes;
            DataPT infoPT;
            DataEng infoEng;
            ArvoreVP *noVP;
            ArvoreBB *noBB;

            if (identificar_traducoes(linha, palavra_ingles, traducoes, &num_traducoes)) {
                // printf("Unidade: %d\n", unidade_atual);
                // printf("Palavra em inglês: %s\n", palavra_ingles);
                // printf("Traduções: ");
                for (int i = 0; i < num_traducoes; i++) {
                    infoPT.palavraPT = alocar_str(50);
                    strcpy(infoPT.palavraPT, traducoes[i]);
                    
                    infoPT.palavrasEng = arvorebb_criar();
                    noVP = arvorevp_inserir(arvore, infoPT);

                    infoEng.palavraIngles = palavra_ingles;
                    infoEng.unidade = lista_cria();
                    noBB = arvorebb_inserir(&(noVP->info.palavrasEng), infoEng);

                    lista_inserir(&(noBB->info.unidade), unidade_atual);

                    // printf("%s%s", traducoes[i], (i == num_traducoes - 1) ? "\n" : ",");
                }
            }
        }
        fclose(arquivo);
    }
}

// void procurar_palavra(const char *nome_arquivo, char *palavra)
// {
// }

int main()
{
    const char *dicionario = "Dicionario.txt";
    ArvoreVP *arvore;
    arvore = arvorevp_criar();
    processar_arquivo(dicionario, &arvore);

    arvorevp_exibir_pre(arvore);
    return 0;
}

//void processar_arquivo(char *nome_arquivo, InglesPortugues **raiz_ingles, PortuguesIngles **raiz_portugues)
/*void processar_arquivo(char *nome_arquivo, ArvBB **raiz_ingles, Arvore23 **raiz_portugues)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if(!arquivo){
        printf("Erro ao abrir o arquivo!\n");
    }
    else{
        char linha[256];
        char unidade_atual[50] = "";
        while (fgets(linha, sizeof(linha), arquivo)){
            linha[strcspn(linha, "\n")] = '\0';

            if(linha[0] == '%')
                strcpy(unidade_atual, linha+1);
            else if(strchr(linha, ':')){
                // pegar palavras em inglês e suas traduções
                char palavra_ingles[50];
                char traducoes[256];
                sscanf(linha, "%[^:]: %[^\n]", palavra_ingles, traducoes);
                // separar as traduções
                char *traducao = strtok(traducoes, ",;");
                char lista_traducoes[10][50];
                int num_traducoes = 0;

                while (traducao){
                    strcpy(lista_traducoes[num_traducoes++], traducao);
                    traducao = strtok(NULL, ",:");
                }

                //inserir na árvore binária (Ingles-Portugues)
                *raiz_ingles = inserir_ingles();
            }
        }
    }
}*/
