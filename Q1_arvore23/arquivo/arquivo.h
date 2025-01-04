#include "../arvore/arvore.h"
char *alocar_str(int tam);

/*
    Função para coletar os dados de um arquivo, armazenando-os em uma árvore
    Tem como parâmetro o nome do arquivo e o a referência da raiz da árvore
*/
void processar_arquivo(char *nome_arquivo, Arvore23 **arvore);