/**
 * Trabalho Prático 2 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 *
///////////////////////////////////////////////////
 *
 * btree/insert.c
 *  
 *  Contem todas as funções utilizadas para inserir
 *      chaves e ponteiros dentro das paginas da
 *      btree.
 * 
 *  insertion() -- Procedimento para inserir chave dentro de uma página
 *  insert_procedure() -- Procedimento recursivo da inserção
 *  insert() -- Procedimento para realizar a inserção no indice
*/

#include <btree/btree.h>
#include <util.h>

/** insertion()
 * 
 *  Faz a inserção da chave e do rrn na pagina passada por parametro
 *  A função não faz checagem se é possível inserir a chave dentro 
 *    da página, isso deve ser feito antes de chamar o procedimento
 * 
 *  Argumentos:
 *      page  -- Página em ram que será inserido a chave
 *      pair -- Par chave-referencia que será inserido na chave
 *      promotedRRN  -- RRN que é inserido adjacente à chave na pagina
 */
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

/** insert_procedure()
 * 
 *  Procedimento recursivo para inserir chave em indice de arvore B.
 *  Não trata casos específico de recriação de raiz do indice.
 *  Retorna se houve ou não promoção: PROMOTED/NOT_PROMOTED
 * 
 *  Argumentos:
 *      RRN -- RRN usado recursivamente para encontrar uma pagina da btree adequada
 *      header -- Header do arquivo da btree. Utilizado para checar meta-informações da btree.
 *      BTree -- Ponteiro para arquivo de indice aberto
 *      promotedPair -- Chave promovida na inserção
 *      promotedRRN -- Ponteiro da pagina associado à chave promovida
 */
int insert_procedure(int RNN, BTree_header *header, FILE *BTree, BTree_pair *promotedPair, int *promotedRNN)
{
    BTree_page *page,           /* Página da btree com o RRN passado por parametro */
               *newPage;        /* Nova página usada se houver split */
    int result,                 /* Resultado da inserção: PROMOTED/NOT_PROMOTED */
        pos;                    /* Posição adequada para a chave no vetor de chaves da página */

    if(RNN == NIL) {         /* Fim da recursão          */
        return PROMOTED;    /* Retorna a chave inserida */
    }                       /*  como chave promovida    */

    page = read_btree_page(RNN, BTree); /* Le a página do RRN atual da recursão */

    /* Iteração na pagina atual para verificar posicao da chave */
    for (pos = 0; pos < MAXKEYS && page->c[pos].key < promotedPair->key && page->c[pos].key != NOKEY; pos++);

    result = insert_procedure(page->p[pos], header, BTree, promotedPair, promotedRNN);

    if(result == NOT_PROMOTED) {    /* Se nenhuma chave não foi promovida   */
        result = NOT_PROMOTED;      /* Sem promoção */

    } else if(page->nroChavesIndexadas < MAXKEYS) {     /* Se alguma chave foi promovida, e existe      */
        insertion(page, promotedPair, *promotedRNN);    /*  espaço dentro da página atual               */
        write_btree_page(RNN, page, BTree);             /* Então, insere a chave na página e atualiza   */
                                                        /*  o arquivo de indice.                        */
        result = NOT_PROMOTED;    /* Sem promoção */

    } else {                                                                    /* Caso não haja espaço dentro da     */
        newPage = split(page, promotedPair, promotedRNN, header->RRNproxNo);    /*  página atual, então faz um split  */
        write_btree_page(RNN, page, BTree);                                     /*  da página atual.                  */
        write_btree_page(*promotedRNN, newPage, BTree);                         /* Depois disso as atuala os arquivos */
        header->RRNproxNo += 1;                                                 /*  de dados e de indice              */

        free(newPage);
        result = PROMOTED;  /* Promoção */
    }

    free(page);
    return result;
}

/** insert()
 * 
 *  Procedimento para inserir nova chave no indice do arquivo
 *      cuida do cenário no qual uma nova raiz deve ser criada
 *      para o arquivo de indice
 * 
 *  Argumentos:
 *      key -- Chave inserida dentro do indice
 *      reference -- Referencia para a chave dentro do arquivo de dados
 *      header -- Header do arquivo da btree. Utilizado para checar meta-informações da btree
 *      BTree -- Ponteiro para arquivo de indice aberto
 */
void insert(int key, long long int reference, BTree_header *header, FILE *BTree)
{
    BTree_pair pair;        /* Estrutura com par chave-referencia para inserir no indice */
    int promotedRNN = -1,   /* Variável para armazenar o ponteiro relacionado à chave na pagina */
        result;             /* Resultado da inserção: PROMOTED/NOT_PROMOTED */

    pair.key = key;             /* Atribuição do par chave-referencia   */
    pair.reference = reference; /*  na estrutura BTree_pair             */

    result = insert_procedure(header->noRaiz, header, BTree, &pair, &promotedRNN);  /* Chamada do procedimento de    */
                                                                                    /*  de inserção utilizando o nó  */
                                                                                    /*  raiz como inicio da recursão */
    
    if (result == PROMOTED) {   /* Caso haja promoção até à página raiz, uma nova pagina raiz é criada */
        create_root(&pair, header->noRaiz, promotedRNN, header, BTree);
    }
}

