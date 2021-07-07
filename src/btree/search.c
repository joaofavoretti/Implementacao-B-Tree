/**
 * Trabalho Prático 2 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <btree/btree.h>
#include <util.h>

int search(int RNN, int key, FILE *bTree, long long int *reference)
{
    /**
     *  Pesquisa pela chave no arquivo de indice arvore-B e retorna
     *  a referencia para posicao do registro no arquivo de dados.
     */

    // Retorna não encontrou se atingir o nó folha
    if(RNN == -1){
        *reference = -1;
        return NOT_FOUND;
    }

    // Le a pagina corrente
    BTree_page *page = read_btree_page(RNN, bTree);
    int pos = 0, result;

    // Posicao em que a chave deveria ficar
    while(pos < 4 && page->c[pos].key < key && page->c[pos].key != -1)
        pos++;

    // Retorna se encontrou e a posicao da chave 
    if(page->c[pos].key == key){
        result = FOUND;
        *reference = page->c[pos].reference;
    }
    // Caso contrario continua a busca na pagina apropriada 
    else
        result = search(page->p[pos], key, bTree, reference);

    free(page);
    return result;
}
