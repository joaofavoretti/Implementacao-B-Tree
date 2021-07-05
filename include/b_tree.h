/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#ifndef _B_TREE_H_
#define _B_TREE_H_

#include <stdio.h>

// Codigos de retorno
#define NOT_FOUND 0
#define FOUND 1
#define NOT_PROMOTED 0
#define PROMOTED 1

/**
 * Struct para armazenar o cabecalho do arquivo indice arvore-B
*/
typedef struct {
    char status;             
    int noRaiz;                
    int RRNproxNo;       
                               
} bTree_header;

/*
 *  Struct para o par chave e referencia 
 */
typedef struct {
    int key;
    long int reference;
} bTree_pair;

/**
 * Struct para armazenar um registro de dados do arquivo indice arvore-B
*/
typedef struct {
    char folha;             
    int nroChavesIndexadas;   
    int RRNdoNo;             
    int p[5];                   
    int c[4];                   
    long int pr[4];             
                                
} bTree_page;


bTree_header *read_bTree_header(FILE *bTree);
void update_bTree_header(bTree_header *header, FILE *bTree);
bTree_page *read_bTree_page(int RNN, FILE *bTree);
void write_bTree_page(int RNN, bTree_page *page, FILE *bTree);
void print_bTree_page(bTree_page *page);

int search(int RNN, int key, FILE *bTree, long int *reference);
int insert(int RNN, bTree_header *header, FILE *bTree, bTree_pair *promotedPair, int *promotedRNN);


#endif