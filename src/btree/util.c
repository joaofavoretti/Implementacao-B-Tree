/**
 * Trabalho Prático 2 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <btree/btree.h>
#include <util.h>

void create_root(BTree_pair *pair, int left, int rigth, BTree_header *header, FILE *bTree)
{
    BTree_page root;
    root.folha = '0';
    if(rigth == -1)
        root.folha = '1';
    root.RRNdoNo = header->RRNproxNo;
    root.nroChavesIndexadas = 1;

    root.c[0].key = pair->key;
    root.c[0].reference = pair->reference;
    root.p[0] = left;
    root.p[1] = rigth;
    for(int i = 1; i < 4; i++){
        root.c[i].key = -1;
        root.c[i].reference = -1;
        root.p[i+1] = -1;
    } 

    write_btree_page(root.RRNdoNo, &root, bTree);
    header->RRNproxNo += 1;
    header->noRaiz = root.RRNdoNo;
}

BTree_page *split(BTree_page *page, BTree_pair *pair, int *promotedRNN, int nextRNN){
    int p_temp[6], c_temp[5];
    long long int pr_temp[5];
    int i;

    // Copia valores para os arrays temporarios
    for(i = 0; i < 4; i++){
        c_temp[i] = page->c[i].key;
        pr_temp[i] = page->c[i].reference;
        p_temp[i] = page->p[i];
    }
    p_temp[i] = page->p[i];

    // Insere a chave nos arrays temporarios
    for(i=4; i > 0 && pair->key < c_temp[i-1]; i--){
        c_temp[i] = c_temp[i-1];
        pr_temp[i] = pr_temp[i-1];
        p_temp[i+1] = p_temp[i];
    }
    c_temp[i] = pair->key;
    pr_temp[i] = pair->reference;
    p_temp[i+1] = *promotedRNN;

    // Cria uma nova pagina
    BTree_page *newPage = malloc(sizeof(BTree_page));
    newPage->nroChavesIndexadas = 2;
    newPage->RRNdoNo = nextRNN;
    if(*promotedRNN == -1)
        newPage->folha = '1';
    else
        newPage->folha = '0';

    page->nroChavesIndexadas = 2;

    // Move metade das chaves para cada pagina
    for(i = 0; i < 2; i++){
        page->c[i].key = c_temp[i];
        page->c[i].reference = pr_temp[i];
        page->p[i] = p_temp[i];

        newPage->c[i].key = c_temp[i+3];
        newPage->c[i].reference = pr_temp[i+3];
        newPage->p[i] = p_temp[i+3];

        page->c[i+2].key = -1;
        page->c[i+2].reference = -1;
        page->p[i+3] = -1;
        newPage->c[i+2].key = -1;
        newPage->c[i+2].reference = -1;
        newPage->p[i+3] = -1;
    }
    page->p[2] = p_temp[2];
    newPage->p[2] = p_temp[5];

    pair->key = c_temp[2];
    pair->reference = pr_temp[2];
    *promotedRNN = nextRNN;

    return newPage;
}
