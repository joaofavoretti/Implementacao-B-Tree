/**
 * Trabalho Prático 2 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 *
///////////////////////////////////////////////////
 *
 * btree/io.c
 *  
 *  Contem as funções de entrada e saida de arquivos relacionados
 *      ao arquivo de indice usado.
 * 
 * read_btree_header() -- Realiza a leitura do cabecalho do arquivo de indice
 * write_btree_header() -- Realiza a escruta do cabecalho do arquivo de indice
 * read_btree_page() -- Faz a leitura de um RRN no arquivo de indice
 * write_btree_page() -- Faz a escrita de um RRN no arquivo de indice
*/

#include <btree/btree.h>
#include <util.h>

/** read_btree_header()
 * 
 *  Funcão lê o cabecalho do arquivo de indice e retorna um ponteiro 
 *      para a estrutura BTree_header contendo a informação lida
 *  Essa função posiciona o ponteiro do arquivo no inicio do arquivo
 *      antes de fazer a leitura, no fim ela mantem o cursor do
 *      arquivo na posição seguinte ao cabecalho do arquivo
 * 
 *  Argumentos:
 *      BTree  -- Ponteiro para o arquivo de indice aberto
 */
BTree_header *read_btree_header(FILE *BTree)
{
    fseek(BTree, 0, SEEK_SET);          /* Posiciona o cursor no começo do arquivo */

    BTree_header* header = (BTree_header *)calloc(1, sizeof(BTree_header));

    fread(&header->status, sizeof(char), 1, BTree);     /* Le status */

    if (header->status == '0') {                        /* Verifica se o arquivo */
        printf("Falha no processamento do arquivo.\n"); /*  eh inconsistente     */
        exit(FAIL_BINARY_STATUS);
    }

    fread(&header->noRaiz, sizeof(int), 1, BTree);
    fread(&header->RRNproxNo, sizeof(int), 1, BTree);

    return header;
}

/** write_btree_header()
 * 
 *  Funcão escreve o cabecalho do arquivo indice baseado em uma estrutura
 *      mantida em RAM.
 *  Essa função posiciona o ponteiro do arquivo no inicio do arquivo
 *      antes de fazer a escrita, no fim ela mantem o cursor do
 *      arquivo na posição seguinte ao cabecalho do arquivo.
 * 
 *  Argumentos:
 *      header -- Ponteiro para a estrutura em RAM com os valores do cabecalho
 *      BTree  -- Ponteiro para o arquivo de indice aberto
 */
void write_btree_header(BTree_header *header, FILE *BTree)
{
    fseek(BTree, 0, SEEK_SET);      /* Posiciona o cursor no começo do arquivo */

    char lixo[BTREE_HEADER_EMPTY_SIZE];
    memset(lixo, LIXO, BTREE_HEADER_EMPTY_SIZE);

    fwrite(&header->status, sizeof(char), 1, BTree);
    fwrite(&header->noRaiz, sizeof(int), 1, BTree);
    fwrite(&header->RRNproxNo, sizeof(int), 1, BTree);
    fwrite(lixo, sizeof(char), BTREE_HEADER_EMPTY_SIZE, BTree);
}

/** read_btree_page()
 * 
 *  Funcão le uma página específica do arquivo de indice.
 *  Essa função posiciona o ponteiro do arquivo no inicio da página
 *      especificada antes de fazer a leitura, no fim ela mantem o
 *      cursor do arquivo na posição seguinte à pagina lida.
 * 
 *  Argumentos:
 *      RRN -- RRN da pagina que será lida
 *      BTree  -- Ponteiro para o arquivo de indice aberto
 */
BTree_page *read_btree_page(int RNN, FILE *BTree)
{
    BTree_page *page = malloc(sizeof(BTree_page));

    fseek(BTree, (RNN + 1) * BTREE_PAGE_SIZE, SEEK_SET); /* Acesssa o registro diretamente pelo RNN */

    fread(&page->folha, sizeof(char), 1, BTree);
    fread(&page->nroChavesIndexadas, sizeof(int), 1, BTree);
    fread(&page->RRNdoNo, sizeof(int), 1, BTree);

    for(int i = 0; i < MAXKEYS; i++){
        fread(&page->p[i], sizeof(int), 1, BTree);
        fread(&page->c[i].key, sizeof(int), 1, BTree);
        fread(&page->c[i].reference, sizeof(long long int), 1, BTree);
    }
    fread(&page->p[MAXKEYS], sizeof(int), 1, BTree);

    return page;
}

/** write_btree_page()
 * 
 *  Funcão escreve uma página específica do arquivo de indice.
 *  Essa função posiciona o ponteiro do arquivo no inicio da página
 *      especificada antes de fazer a escrita, no fim ela mantem o
 *      cursor do arquivo na posição seguinte à pagina escrita.
 * 
 *  Argumentos:
 *      RRN -- RRN da pagina que será lida
 *      page -- Página mantida em RAM com os valores que serão escritos
 *      BTree  -- Ponteiro para o arquivo de indice aberto
 */
void write_btree_page(int RNN, BTree_page *page, FILE *BTree)
{
    fseek(BTree, (RNN + 1) * BTREE_PAGE_SIZE, SEEK_SET);

    fwrite(&page->folha, sizeof(char), 1, BTree);
    fwrite(&page->nroChavesIndexadas, sizeof(int), 1, BTree);
    fwrite(&page->RRNdoNo, sizeof(int), 1, BTree);

    for(int i = 0; i < MAXKEYS; i++){
        fwrite(&page->p[i], sizeof(int), 1, BTree);
        fwrite(&page->c[i].key, sizeof(int), 1, BTree);
        fwrite(&page->c[i].reference, sizeof(long long int), 1, BTree);
    }
    fwrite(&page->p[MAXKEYS], sizeof(int), 1, BTree);
}
