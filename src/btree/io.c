/**
 * Trabalho Prático 2 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <btree/btree.h>
#include <util.h>

BTree_header *read_btree_header(FILE *bTree)
{
    /**
     *  Le cabecalho do arquivo de indice
     */
    /* Posiciona no começo do arquivo */
    fseek(bTree, 0, SEEK_SET);

    BTree_header* header = (BTree_header *)calloc(1, sizeof(BTree_header));
    /* Le status */
    fread(&header->status, sizeof(char), 1, bTree);

    /* Verifica se o arquivo eh inconsistente */
    if (header->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        exit(FAIL_BINARY_STATUS);
    }

    fread(&header->noRaiz, sizeof(int), 1, bTree);
    fread(&header->RRNproxNo, sizeof(int), 1, bTree);

    return header;
}


void write_btree_header(BTree_header *header, FILE *bTree)
{
    /**
     *  Atualiza cabecalho do arquivo de indice
     */
    /* Posiciona no começo do arquivo */
    fseek(bTree, 0, SEEK_SET);

    char lixo[69];
    memset(lixo, '@', 68);

    fwrite(&header->status, sizeof(char), 1, bTree);
    fwrite(&header->noRaiz, sizeof(int), 1, bTree);
    fwrite(&header->RRNproxNo, sizeof(int), 1, bTree);
    fwrite(lixo, sizeof(char), 68, bTree);
}


BTree_page *read_btree_page(int RNN, FILE *bTree)
{
    /**
     *  Le uma pagina do arquivo de indice pelo RNN 
     */
    BTree_page *page = malloc(sizeof(BTree_page));
    
    /* Acesssa o registro diretamente pelo RNN */
    fseek(bTree, (RNN+1)*77, SEEK_SET);

    fread(&page->folha, sizeof(char), 1, bTree);
    fread(&page->nroChavesIndexadas, sizeof(int), 1, bTree);
    fread(&page->RRNdoNo, sizeof(int), 1, bTree);

    for(int i = 0; i < 4; i++){
        fread(&page->p[i], sizeof(int), 1, bTree);
        fread(&page->c[i].key, sizeof(int), 1, bTree);
        fread(&page->c[i].reference, sizeof(long long int), 1, bTree);
    }
    fread(&page->p[4], sizeof(int), 1, bTree);

    return page;
}


void write_btree_page(int RNN, BTree_page *page, FILE *bTree)
{
    fseek(bTree, (RNN+1)*77, SEEK_SET);

    fwrite(&page->folha, sizeof(char), 1, bTree);
    fwrite(&page->nroChavesIndexadas, sizeof(int), 1, bTree);
    fwrite(&page->RRNdoNo, sizeof(int), 1, bTree);

    for(int i = 0; i < 4; i++){
        fwrite(&page->p[i], sizeof(int), 1, bTree);
        fwrite(&page->c[i].key, sizeof(int), 1, bTree);
        fwrite(&page->c[i].reference, sizeof(long long int), 1, bTree);
    }
    fwrite(&page->p[4], sizeof(int), 1, bTree);
}
