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

veiculo_header *read_binary_veiculo_header(FILE *binFilePointer)
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

    if (header->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        exit(FAIL_BINARY_STATUS);
    }

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

veiculo_data *read_binary_veiculo_data(veiculo_header *header, FILE *binFilePointer)
{
    /*
     *  Função para ler um registro do binario fornecido por parametro
     *  Retorna uma struct com os dados do registro
     */

    veiculo_data *data = (veiculo_data *)calloc(1, sizeof(veiculo_data));
    alloc_check(header, "struct veiculo_data não foi alocado com sucesso\n");

    fread(&data->removido, sizeof(char), 1, binFilePointer);
    fread(&data->tamanhoRegistro, sizeof(int), 1, binFilePointer);

    //Verifica se o registro esta logicamente removido
    if(data->removido == '0')
        return data;

    fread(data->prefixo, sizeof(char), 5, binFilePointer);
    fread(data->data, sizeof(char), 10, binFilePointer);
    fread(&data->quantidadeLugares, sizeof(int), 1, binFilePointer);
    fread(&data->codLinha, sizeof(int), 1, binFilePointer);
    fread(&data->tamanhoModelo, sizeof(int), 1, binFilePointer);
    fread(data->modelo, sizeof(char), data->tamanhoModelo, binFilePointer);
    fread(&data->tamanhoCategoria, sizeof(int), 1, binFilePointer);
    fread(data->categoria, sizeof(char), data->tamanhoCategoria, binFilePointer);
    
    return data;
}

linha_header *read_binary_linha_header(FILE *binFilePointer)
{
    /**
     * Funcao para ler o header do binario fornecido por parametro.
     * Posiciona o cursor na posicao 0 do arquivo para realizar a leitura. Apos a leitura, posiciona o cursor no primeiro byte do primeiro registro.
     * 
     * @param binFilePointer Ponteiro aberto para o arquivo binario do veiculo
     */

    linha_header *header = (linha_header *)calloc(sizeof(linha_header), 1 * sizeof(linha_header));
    alloc_check(header, "struct linha_header não foi alocado com sucesso\n");

    fseek(binFilePointer, 0, SEEK_SET);

    fread(&header->status, sizeof(char), 1, binFilePointer);

    if (header->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        exit(FAIL_BINARY_STATUS);
    }

    fread(&header->byteProxReg, sizeof(long long int), 1, binFilePointer);
    fread(&header->nroRegistros, sizeof(int), 1, binFilePointer);
    fread(&header->nroRegRemovidos, sizeof(int), 1, binFilePointer);
    fread(header->descreveCodigo, sizeof(char), 15, binFilePointer);
    fread(header->descreveCartao, sizeof(char), 13, binFilePointer);
    fread(header->descreveNome, sizeof(char), 13, binFilePointer);
    fread(header->descreveCor, sizeof(char), 24, binFilePointer);

    return header;
}

linha_data *read_binary_linha_data(linha_header *header, FILE *binFilePointer)
{
    /*
     *  Função para ler um registro do binario fornecido por parametro
     *  Retorna uma struct com os dados do registro
     */

    linha_data *data = (linha_data *)calloc(1, sizeof(linha_data));
    alloc_check(header, "struct linha_data não foi alocado com sucesso\n");
    
    fread(&data->removido, sizeof(char), 1, binFilePointer);
    fread(&data->tamanhoRegistro, sizeof(int), 1, binFilePointer);

    //Verifica se o registro esta logicamente removido
    if(data->removido == '0')
        return data;

    fread(&data->codLinha, sizeof(int), 1, binFilePointer);
    fread(&data->aceitaCartao, sizeof(char), 1, binFilePointer);
    fread(&data->tamanhoNome, sizeof(int), 1, binFilePointer);
    fread(data->nomeLinha, sizeof(char), data->tamanhoNome, binFilePointer);
    fread(&data->tamanhoCor, sizeof(int), 1, binFilePointer);
    fread(data->corLinha, sizeof(char), data->tamanhoCor, binFilePointer);

    return data;
}
