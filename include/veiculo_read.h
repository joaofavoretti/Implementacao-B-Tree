/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#ifndef _VEICULO_READ_H_
#define _VEICULO_READ_H_

#include <stdio.h>

#define VEICULO_HEADER_SIZE 174     /* Tamanho do cabecalho do ao arquivo binario veiculo */
                                    /* Usado para calcular o offset do primeiro registro de dados */

/**
 * Struct para armazenar o cabecalho do arquivo tipo veiculo
*/
typedef struct {
    char status;                // 1 byte
    long long int byteProxReg;  // 8 bytes
    int nroRegistros;           // 4 bytes
    int nroRegRemovidos;        // 4 bytes
    char descrevePrefixo[32];   // 18 bytes field
    char descreveData[64];      // 35 bytes field
    char descreveLugares[64];   // 42 bytes
    char descreveLinha[32];     // 26 bytes
    char descreveModelo[32];    // 17 bytes
    char descreveCategoria[32]; // 20 bytes
                                // 174 bytes
} veiculo_header;


#define VEICULO_DATA_DEFAULT_SIZE 31    /* Tamanho da struct veiculo_data  se "modelo" e "categoria" tiverem tamanho 0 e sem contar o campo "removido" */
                                        /* Usado para calcular o tamanho do registro */

/**
 * Struct para armazenar um registro de dados por vez
*/
typedef struct {
    char removido;              // 1 byte
    int tamanhoRegistro;        // 4 bytes
    char prefixo[8];            // 5 bytes
    char data[16];              // 10 bytes
    int quantidadeLugares;      // 4 bytes
    int codLinha;               // 4 bytes
    int tamanhoModelo;          // 4 bytes
    char modelo[128];           // Tamanho variavel (máximo 128 bytes)
    int tamanhoCategoria;       // 4 bytes
    char categoria[128];        // Tamanho variavel (maximo 128 bytes)
} veiculo_data;

/**
 * Arquivo para armazenar:
 *  - Ponteiro para o arquivo binario
 *  - Ponteiro para a struct de cabecalho que será atualizada quando a struct for liberada da memoria
 *  - Ponteiro para a ultima struct de dados manipulado (veiculo_data)
*/
typedef struct {
    FILE *fp;                   // File pointer para o arquivo de registro de dados
    veiculo_header *header;     // Ponteiro para o header do arquivo mantido em RAM
    veiculo_data *data;         // Ponteiro para 1 registro de dados mantido em RAM
} veiculo_file;

veiculo_file *read_csv_veiculo_file (char *csvFileName, char *binFileName);
veiculo_header *read_csv_veiculo_header(FILE *csvFilePointer, FILE *binFilePointer);
void update_binary_veiculo_header(veiculo_header *header, FILE *binFilePointer);
veiculo_data *read_csv_veiculo_data(veiculo_header *header, FILE *csvFilePointer, FILE *binFilePointer);
void append_binary_veiculo_data (veiculo_header *header, veiculo_data *data, FILE *binFilePointer);
void close_binary_veiculo_file(veiculo_file* file);

#endif
