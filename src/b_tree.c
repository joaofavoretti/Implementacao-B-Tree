/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <b_tree.h>
#include <util.h>

bTree_header *read_bTree_header(FILE *bTree)
{
    /**
     *  Le cabecalho do arquivo de indice
     */
    /* Posiciona no começo do arquivo */
    fseek(bTree, 0, SEEK_SET);

    bTree_header* header = (bTree_header *)calloc(1, sizeof(bTree_header));
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


void update_bTree_header(bTree_header *header, FILE *bTree)
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


bTree_page *read_bTree_page(int RNN, FILE *bTree)
{
    /**
     *  Le uma pagina do arquivo de indice pelo RNN 
     */
    bTree_page *page = malloc(sizeof(bTree_page));
    
    /* Acesssa o registro diretamente pelo RNN */
    fseek(bTree, (RNN+1)*77, SEEK_SET);

    fread(&page->folha, sizeof(char), 1, bTree);
    fread(&page->nroChavesIndexadas, sizeof(int), 1, bTree);
    fread(&page->RRNdoNo, sizeof(int), 1, bTree);

    for(int i = 0; i < 4; i++){
        fread(&page->p[i], sizeof(int), 1, bTree);
        fread(&page->c[i], sizeof(int), 1, bTree);
        fread(&page->pr[i], sizeof(long int), 1, bTree);
    }
    fread(&page->p[4], sizeof(int), 1, bTree);

    return page;
}


void print_bTree_page(bTree_page *page){
    printf("Folha: %c\n", page->folha);
    printf("RRN do No: %d\n", page->RRNdoNo);
    printf("Nro de Chaves: %d\n", page->nroChavesIndexadas);
    printf("Chaves: \n");
    for(int i = 0; i < 4; i++){
        printf("(%d, %ld) ", page->c[i], page->pr[i]);
    }
    printf("\n");
    printf("Filhos: \n");
    for(int i = 0; i < 5; i++){
        printf("%d ", page->p[i]);
    }
    printf("\n");
}


void write_bTree_page(int RNN, bTree_page *page, FILE *bTree)
{
    fseek(bTree, (RNN+1)*77, SEEK_SET);

    fwrite(&page->folha, sizeof(char), 1, bTree);
    fwrite(&page->nroChavesIndexadas, sizeof(int), 1, bTree);
    fwrite(&page->RRNdoNo, sizeof(int), 1, bTree);

    for(int i = 0; i < 4; i++){
        fwrite(&page->p[i], sizeof(int), 1, bTree);
        fwrite(&page->c[i], sizeof(int), 1, bTree);
        fwrite(&page->pr[i], sizeof(long int), 1, bTree);
    }
    fwrite(&page->p[4], sizeof(int), 1, bTree);
}


int search(int RNN, int key, FILE *bTree, long int *reference)
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
    bTree_page *page = read_bTree_page(RNN, bTree);
    int pos = 0, result;

    // Posicao em que a chave deveria ficar
    while(pos < 4 && page->c[pos] < key && page->c[pos] != -1)
        pos++;

    // Retorna se encontrou e a posicao da chave 
    if(page->c[pos] == key){
        result = FOUND;
        *reference = page->pr[pos];
    }
    // Caso contrario continua a busca na pagina apropriada 
    else
        result = search(page->p[pos], key, bTree, reference);

    free(page);
    return result;
}


void insertion(bTree_page *page, bTree_pair *pair, int promotedRNN){
    int i;
    for(i = page->nroChavesIndexadas; i > 0 && pair->key < page->c[i-1]; i--){
        page->c[i] = page->c[i-1];
        page->pr[i] = page->pr[i-1];
        page->p[i+1] = page->p[i];
    }
    
    page->c[i] = pair->key;
    page->pr[i] = pair->reference;
    page->p[i+1] = promotedRNN;
    page->nroChavesIndexadas += 1;
}


bTree_page *split(bTree_page *page, bTree_pair *pair, int *promotedRNN, int nextRNN){
    int p_temp[6], c_temp[5];
    long int pr_temp[5];
    int i;

    // Copia valores para os arrays temporarios
    for(i = 0; i < 4; i++){
        c_temp[i] = page->c[i];
        pr_temp[i] = page->pr[i];
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
    bTree_page *newPage = malloc(sizeof(bTree_page));
    newPage->nroChavesIndexadas = 2;
    newPage->RRNdoNo = nextRNN;
    if(*promotedRNN == -1)
        newPage->folha = '1';
    else
        newPage->folha = '0';

    page->nroChavesIndexadas = 2;

    // Move metade das chaves para cada pagina
    for(i = 0; i < 2; i++){
        page->c[i] = c_temp[i];
        page->pr[i] = pr_temp[i];
        page->p[i] = p_temp[i];

        newPage->c[i] = c_temp[i+3];
        newPage->pr[i] = pr_temp[i+3];
        newPage->p[i] = p_temp[i+3];

        page->c[i+2] = -1;
        page->pr[i+2] = -1;
        page->p[i+3] = -1;
        newPage->c[i+2] = -1;
        newPage->pr[i+2] = -1;
        newPage->p[i+3] = -1;
    }
    page->p[2] = p_temp[2];
    newPage->p[2] = p_temp[5];

    pair->key = c_temp[2];
    pair->reference = pr_temp[2];
    *promotedRNN = nextRNN;

    // printf("\nPagina:");
    // for(i = 0; i < 4;  i++){
    //     printf("%d ", page->c[i]);
    // }
    // printf("\nPagina nova: ");
    // for(i = 0; i < 4;  i++){
    //     printf("%d ", newPage->c[i]);
    // }
    // printf("\n");

    return newPage;
}


int insert(int RNN, bTree_header *header, FILE *bTree, bTree_pair *promotedPair, int *promotedRNN)
{
    // Promove a chave original quando atinge o no folha
    if(RNN == -1){ 
        *promotedRNN = -1;
        return PROMOTED;
    }

    // Le a pagina corrente
    bTree_page *page = read_bTree_page(RNN, bTree);
    // printf("\nRNN atual: %d\n", page->RRNdoNo);

    // Posicao em que a chave deveria ficar
    int pos = 0;
    while(pos < 4 && page->c[pos] < promotedPair->key && page->c[pos] != -1){
        pos++;
    }
    // for(int i = 0; i < 4; i++){
    //     printf("(%d, %ld) ", page->c[i], page->pr[i]);
    // }
    // printf("\n");

    // printf("Next RNN: %d\n", page->p[pos]);
    int result = insert(page->p[pos], header, bTree, promotedPair, promotedRNN);
    // printf("Resultado: %d\n", result);

    if(result == NOT_PROMOTED) 
        result = NOT_PROMOTED;
    else if(page->nroChavesIndexadas < 4){
        // printf("Insertion...\n");
        insertion(page, promotedPair, *promotedRNN);
        write_bTree_page(RNN, page, bTree);
        result = NOT_PROMOTED;
    } 
    else {
        // printf("Split...\n");
        bTree_page *newPage = split(page, promotedPair, promotedRNN, header->RRNproxNo);
        write_bTree_page(RNN, page, bTree);
        write_bTree_page(*promotedRNN, newPage, bTree);
        free(newPage);
        result = PROMOTED;
        header->RRNproxNo += 1;
    }

    free(page);
    return result;
}