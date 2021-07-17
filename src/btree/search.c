/**
 * Trabalho Prático 3 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 *
///////////////////////////////////////////////////
 *
 * btree/search.c
 *  
 *  Contem todas as funções utilizadas para procurar
 *      por chaves dentro do indice
 * 
 *  search() -- Procedimento para procurar pelas chaves dentro do indice
*/

#include <btree/btree.h>
#include <util.h>

/** search()
 * 
 *  Pesquisa pela chave no arquivo de indice arvore-B e retorna
 *   a referencia para posicao do registro no arquivo de dados.
 *  Retorna o status da pesquisa: FOUND/NOT_FOUND
 * 
 *  Argumentos:
 *      RRN     -- RRN usado para pesquisa recursiva entre as paginas da arvore B
 *      key     -- Chave pesquisada
 *      BTree   -- Ponteiro para a estrutura FILE criada ao abrir o arquivo de indice
 *      reference -- Ponteiro para onde será armazenado o resultado da busca
 */
int search(int RNN, int key, FILE *BTree, long long int *reference)
{
    if(RNN == NIL) {        /* Caso RRN não seja uma referencia para outra pagina  */
        *reference = NIL;   /*  da BTree (NIL), então não existe a chave dentro do */
        return NOT_FOUND;   /*  arquivo de indice.                                 */
    }

    BTree_page *page = read_btree_page(RNN, BTree); /* Lê pagina da BTree para busca */
    int pos = 0, result;

    /* Interação sequencial na página da BTree para buscar pela chave */
    for (pos = 0; pos < MAXKEYS && page->c[pos].key < key && page->c[pos].key != NOKEY; pos++);

    if(page->c[pos].key == key){        /* Se a chave foi encontrada, retorna */
        result = FOUND;                 /*  FOUND e atribui referencia        */
        *reference = page->c[pos].reference;
    } else {
        result = search(page->p[pos], key, BTree, reference);
    }

    free(page);
    return result;
}
