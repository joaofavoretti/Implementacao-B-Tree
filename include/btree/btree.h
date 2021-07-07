/**
 * Trabalho Prático 2 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#ifndef INCLUDE_BTREE_BTREE_H
#define INCLUDE_BTREE_BTREE_H

#define MAXKEYS 4                   /* Quantidade de chaves por pagina btree */
#define MINKEYS MAXKEYS/2
#define NIL -1
#define NOKEY -1

#define NO 0
#define YES 1
#define LIXO '@'

#define NOT_FOUND 0                 /* Código chave não encontrada */
#define FOUND 1                     /* Código chave encontrada */
#define NOT_PROMOTED 0              /* Retorno chave Não promovida */
#define PROMOTED 1                  /* Retorno chave promovida */

#define BTREE_PAGE_SIZE 77          /* Tamanho pagina btree */
#define BTREE_HEADER_SIZE 77        /* Tamanho registro cabeçalho */

#include <stdio.h>

typedef struct {        /* Struct para armazenar o cabecalho do arquivo indice arvore-B */
    char status;        /* Indica se o arquivo é valido ou não */
    int noRaiz;         /* Indica o RRN do nó raiz */
    int RRNproxNo;      /* Indica o RRN que poderá ser usado ao criar mais uma página */
} BTree_header;
        
typedef struct {            /* Struct para o par chave e referencia */
    int key;                /* Chave indexada */
    long int reference;     /* Byteoffset referencia no arquivo armazenado */
} BTree_pair;


typedef struct {                /* Struct para armazenar um registro de dados do arquivo indice arvore-B */
    char folha;                 /* Representa se a página é um nó folha ou não '1'/'0' */
    int nroChavesIndexadas;     /* Quantidade de chaves ja armazenadas nessa chave */
    int RRNdoNo;                /* Numero do RRN do nó criado */
    BTree_pair c[MAXKEYS];      /* Vetor de chaves da página  */
    int p[MAXKEYS + 1];         /* Vetor de ponteiros da página */
} BTree_page;


BTree_header *read_bTree_header(FILE *bTree);
void update_bTree_header(BTree_header *header, FILE *bTree);
BTree_page *read_bTree_page(int RNN, FILE *bTree);
void write_bTree_page(int RNN, BTree_page *page, FILE *bTree);
void print_bTree_page(BTree_page *page);

int search(int RNN, int key, FILE *bTree, long long int *reference);
void insert(int key, long long int reference, BTree_header *header, FILE *bTree);


#endif