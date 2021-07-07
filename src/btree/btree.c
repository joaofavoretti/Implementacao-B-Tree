/**
 * Trabalho Prático 2 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <btree/btree.h>
#include <util.h>

BTree_header *read_bTree_header(FILE *bTree)
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


void update_bTree_header(BTree_header *header, FILE *bTree)
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


BTree_page *read_bTree_page(int RNN, FILE *bTree)
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


void write_bTree_page(int RNN, BTree_page *page, FILE *bTree)
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
    BTree_page *page = read_bTree_page(RNN, bTree);
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

    write_bTree_page(root.RRNdoNo, &root, bTree);
    header->RRNproxNo += 1;
    header->noRaiz = root.RRNdoNo;
}


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


int insert_procedure(int RNN, BTree_header *header, FILE *bTree, BTree_pair *promotedPair, int *promotedRNN)
{
    // Promove a chave original quando atinge o no folha
    if(RNN == -1)
        return PROMOTED;

    // Le a pagina corrente
    BTree_page *page = read_bTree_page(RNN, bTree);

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
        write_bTree_page(RNN, page, bTree);
        result = NOT_PROMOTED;
    } 
    else {
        BTree_page *newPage = split(page, promotedPair, promotedRNN, header->RRNproxNo);
        write_bTree_page(RNN, page, bTree);
        write_bTree_page(*promotedRNN, newPage, bTree);
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