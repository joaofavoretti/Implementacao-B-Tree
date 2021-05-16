/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <read_binary.h>
#include <alloc_check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

veiculo_header *read_binary_header(FILE *binFilePointer)
{
    /**
     * Funcao para ler o header do binario fornecido por parametro.
     * Posiciona o cursor na posicao 0 do arquivo para realizar a leitura. Apos a leitura, posiciona o cursor no primeiro byte do primeiro registro.
     * 
     * @param binFilePointer Ponteiro aberto para o arquivo binario do veiculo
     */

    veiculo_header *header = (veiculo_header *)calloc(sizeof(veiculo_header), 1 * sizeof(veiculo_header));
    alloc_check(header, "struct veiculo_header não foi alocado com sucesso\n");

    fseek(binFilePointer, 0, SEEK_SET);

    fread(&header->status, sizeof(char), 1, binFilePointer);
    fread(&header->byteProxReg, sizeof(long long int), 1, binFilePointer);
    fread(&header->nroRegistros, sizeof(int), 1, binFilePointer);
    fread(&header->nroRegRemovidos, sizeof(int), 1, binFilePointer);
    fread(header->descrevePrefixo, sizeof(char), 18, binFilePointer);
    fread(header->descreveData, sizeof(char), 35, binFilePointer);
    fread(header->descreveLugares, sizeof(char), 42, binFilePointer);
    fread(header->descreveLinha, sizeof(char), 26, binFilePointer);
    fread(header->descreveModelo, sizeof(char), 17, binFilePointer);
    fread(header->descreveCategoria, sizeof(char), 20, binFilePointer);

    return header;
}
