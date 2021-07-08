/**
 * Trabalho Prático 2 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <btree/btree.h>
#include <util.h>

void insertion(BTree_page *page, BTree_pair *pair, int promotedRNN){
    int i;
    for(i = page->nroChavesIndexadas; i > 0 && pair->key < page->c[i-1].key; i--){
        page->c[i].key = page->c[i-1].key;
        page->c[i].reference = page->c[i-1].reference;
        page->p[i+1] = page->p[i];
    }
    
    page->c[i].key = pair->key;
    page->c[i].reference = pair->reference;
    page->p[i+1] = promotedRNN;
    page->nroChavesIndexadas += 1;
}

int insert_procedure(int RNN, BTree_header *header, FILE *bTree, BTree_pair *promotedPair, int *promotedRNN)
{
    // Promove a chave original quando atinge o no folha
    if(RNN == -1)
        return PROMOTED;

    // Le a pagina corrente
    BTree_page *page = read_btree_page(RNN, bTree);

    // Posicao em que a chave deveria ficar
    int pos = 0;
    while(pos < 4 && page->c[pos].key < promotedPair->key && page->c[pos].key != -1){
        pos++;
    }

    int result = insert_procedure(page->p[pos], header, bTree, promotedPair, promotedRNN);

    if(result == NOT_PROMOTED) 
        result = NOT_PROMOTED;
    else if(page->nroChavesIndexadas < 4){
        insertion(page, promotedPair, *promotedRNN);
        write_btree_page(RNN, page, bTree);
        result = NOT_PROMOTED;
    } 
    else {
        BTree_page *newPage = split(page, promotedPair, promotedRNN, header->RRNproxNo);
        write_btree_page(RNN, page, bTree);
        write_btree_page(*promotedRNN, newPage, bTree);
        free(newPage);
        result = PROMOTED;
        header->RRNproxNo += 1;
    }

    free(page);
    return result;
}


void insert(int key, long long int reference, BTree_header *header, FILE *bTree)
{
    BTree_pair pair;
    int promotedRNN = -1;

    // Par que sera inserido
    pair.key = key;
    pair.reference = reference;

    // Insere o par com o procedimento recursivo de insercao
    int result = insert_procedure(header->noRaiz, header, bTree, &pair, &promotedRNN);
    // Se necessario cria uma nova raiz
    if(result == PROMOTED)
        create_root(&pair, header->noRaiz, promotedRNN, header, bTree);
}

