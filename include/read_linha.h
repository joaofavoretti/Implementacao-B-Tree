/**
 * Trabalho Prático 2 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#ifndef _LINHA_READ_H_
#define _LINHA_READ_H_

#include <util.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FAIL_BINARY_STATUS 0
#define LINHA_HEADER_SIZE 82     /* Tamanho do cabecalho do ao arquivo binario linha */

/**
 * Struct para armazenar o cabecalho do arquivo tipo linha
*/
typedef struct {
    char status;                // 1 byte
    long long int byteProxReg;  // 8 bytes
    int nroRegistros;           // 4 bytes
    int nroRegRemovidos;        // 4 bytes
    char descreveCodigo[32];    // 15 bytes field
    char descreveCartao[16];    // 13 bytes field
    char descreveNome[16];      // 13 bytes
    char descreveCor[32];       // 24 bytes
                                // 82 bytes
} linha_header;


#define LINHA_DATA_DEFAULT_SIZE 13 /* Tamanho padrao struct linha_data sem contar campo: "removido", "tamanhoRegistro", "nomeLinha", "corLinha" */

/**
 * Struct para armazenar um registro de dados do tipo linha por vez
*/
typedef struct {
    char removido;              // 1 byte
    int tamanhoRegistro;        // 4 bytes
    int codLinha;               // 4 bytes
    char aceitaCartao;          // 1 byte
    int tamanhoNome;            // 4 bytes
    char nomeLinha[128];        // Tamanho variavel (máximo 128 bytes)
    int tamanhoCor;             // 4 bytes
    char corLinha[128];         // Tamanho variavel (maximo 128 bytes)
} linha_data;

linha_header *read_csv_linha_header(FILE *csvFilePointer);
linha_data *read_csv_linha_data(linha_header *header, FILE *csvFilePointer);

linha_header *read_binary_linha_header(FILE *binFilePointer);
linha_data *read_binary_linha_data(FILE *binFilePointer);

linha_data *read_stdin_linha_data(linha_header *header);

#endif