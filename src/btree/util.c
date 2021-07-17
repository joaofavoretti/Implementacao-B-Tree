/**
 * Trabalho Prático 3 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 *
///////////////////////////////////////////////////
 *
 * btree/util.c
 *  
 *  Contem todas as funções utilitárias para os procedimentos
 *      basicos de search e insert da arvore B.
 * 
 *  create_root() -- Função para criar uma nova raiz para o arquivo de indice
 *  page_init() -- Função para zerar os valores de chave e ponteiro de uma pagina BTree
 *  split() -- Procedimento recursivo da inserção
*/

#include <btree/btree.h>
#include <util.h>

/** create_root()
 * 
 *  Funcão sem retorno que cria uma nova raiz para o arquivo de indices e
 *      atualiza o header no arquivo de indice.
 *  Aceita como parametro a chave que será a nova raiz e os ponteiros
 *      left e right.
 * 
 *  Argumentos:
 *      pair -- Par chave-referencia que será inserido na pagina
 *      left -- Ponteiro à esquerda para outra pagina da btree
 *      right -- Ponteiro à direita para outra pagina da btree
 *      header -- Estrutura com cabecalho do arquivo de indice
 *      BTree -- Ponteiro para o arquivo de indice
 */
void create_root(BTree_pair *pair, int left, int right, BTree_header *header, FILE *BTree)
{
    BTree_page root;    /* Página criada para armazenar as informações  */
                        /*  nova raiz                                   */

    page_init(&root);   /* Zera as informações de chave */
                        /*  e ponteiro da nova pagina   */

    root.folha = right == NIL ? '1' : '0';    /* Atribui as informações para  */
    root.RRNdoNo = header->RRNproxNo;         /*  criar uma nova pagina raiz  */
    root.nroChavesIndexadas = 1;

    root.c[0].key = pair->key;
    root.c[0].reference = pair->reference;
    root.p[0] = left;
    root.p[1] = right;

    write_btree_page(root.RRNdoNo, &root, BTree);   /* Atualiza a raiz no arquivo de */
                                                    /*  indice                       */
    header->RRNproxNo += 1;
    header->noRaiz = root.RRNdoNo;
}

/** page_init()
 * 
 *  Função sem retorno para zerar os ponteiro e as chaves de uma
 *      pagina da BTree
 * 
 *  *IMPORTANTE*: Não atribui valor para os meta-dados da página
 *      BTree, por exemplo: "folha", "nroChavesIndexadas" e "RRNdoNo".
 * 
 *  Argumentos:
 *      page  -- Página para zerar os valores
 */
void page_init(BTree_page *page) {
    int i;
    
    for(i = 0; i < MAXKEYS; i++){
        page->c[i].key = NOKEY;
        page->c[i].reference = NIL;
        page->p[i] = NIL;
    }

    page->p[MAXKEYS] = NIL;
}

/** split()
 * 
 *  Função utilizada para dividir
 * 
 *  Argumentos:
 *      page  -- Página em ram que será inserido a chave
 *      pair -- Par chave-referencia que será inserido na chave
 *      promotedRRN  -- RRN que é inserido adjacente à chave na pagina
 *      nextRRN -- Numero do RRN da página criada
 */
BTree_page *split(BTree_page *page, BTree_pair *pair, int *promotedRNN, int nextRNN){

    int i;
    int p_temp[MAXKEYS + 2];                  /* Vetor de ponteiros temporário para realizar o split */
    BTree_pair c_temp[MAXKEYS + 1];           /* Vetor de chaves temporário para realizar o split */
    BTree_page *newPage;                      /* Nova página criada para o split */

    for(i = 0; i < MAXKEYS; i++){                   /* Copiar os valores da página antiga */
        c_temp[i].key = page->c[i].key;             /*  para os arrays temporários        */
        c_temp[i].reference = page->c[i].reference;
        p_temp[i] = page->p[i];
    }
    p_temp[i] = page->p[i];

    for(i = MAXKEYS; i > 0 && pair->key < c_temp[i-1].key; i--){    /* Insere chave e ponteiros */
        c_temp[i].key = c_temp[i - 1].key;                          /*  nos arrays temporários  */
        c_temp[i].reference = c_temp[i - 1].reference;
        p_temp[i + 1] = p_temp[i];
    }
    c_temp[i].key = pair->key;
    c_temp[i].reference = pair->reference;
    p_temp[i + 1] = *promotedRNN;

    newPage = malloc(sizeof(BTree_page));
    page_init(newPage);

    for(i = 0; i < MINKEYS; i++){                       /* Move a primeira metade das chaves e ponteiros */
        page->c[i].key = c_temp[i].key;                 /*  para a página antiga. E a segunda metade das */
        page->c[i].reference = c_temp[i].reference;     /*  chaves e ponteiros para a nova página        */
        page->p[i] = p_temp[i];

        newPage->c[i].key = c_temp[i + 1 + MINKEYS].key;
        newPage->c[i].reference = c_temp[i + 1 + MINKEYS].reference;
        newPage->p[i] = p_temp[i + 1 + MINKEYS];

        page->c[i + 2].key = NOKEY;           /* Zera os demais valores da página */
        page->c[i + 2].reference = NIL;       /*  antiga.                         */
        page->p[i + 3] = NIL;
    }
    page->p[MINKEYS] = p_temp[MINKEYS];
    newPage->p[MINKEYS] = p_temp[i + 1 + MINKEYS];

    newPage->RRNdoNo = nextRNN;                         /* Atribui novos meta-dados das páginas    */
    newPage->nroChavesIndexadas = MAXKEYS - MINKEYS;    /*  antiga e nova para os valores corretos */
    page->nroChavesIndexadas = MINKEYS;
    newPage->folha = *promotedRNN == NIL ? '1' : '0';

    pair->key = c_temp[MINKEYS].key;                /* Atribuindo a chave que será promovida */
    pair->reference = c_temp[MINKEYS].reference;    
    *promotedRNN = nextRNN;

    return newPage;
}
