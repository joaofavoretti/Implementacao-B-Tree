/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <veiculo_read.h>
#include <is_null_field.h>
#include <alloc_check.h>
#include <util.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

veiculo_file *read_csv_veiculo_file (char *csvFileName, char *binFileName)
{
    /**
     * Funcao principal para ler um arquivo CSV veiculo e passar para o arquivo binFileName.
     * 
     * @param csvFilename Nome do arquivo CSV do tipo veiculo que será lido
     * @param binFileName Nome do arquivo binario que as informacoes do arquivo CSV de veiculo
    */

    /* Abre o arquivo CSV */
    FILE *csvFilePointer = fopen (csvFileName, "r");
    alloc_check(csvFilePointer, "Falha no processamento do arquivo.\n");

    veiculo_file *file = (veiculo_file *)calloc(sizeof(veiculo_file), 1 * sizeof(veiculo_file));
    alloc_check(file, "struct veiculo_file não alocada com sucesso\n");

    file->fp = fopen(binFileName, "w+");
    alloc_check(csvFilePointer, "ponteiro para binario não foi alocado com sucesso\n");

    file->header = read_csv_veiculo_header(csvFilePointer, file->fp);

    file->data = read_csv_veiculo_data(file->header, csvFilePointer, file->fp);

    fclose(csvFilePointer);
    return file;
}

veiculo_header *read_csv_veiculo_header(FILE *csvFilePointer, FILE *binFilePointer)
{
    /**
     * Funcao para ler a primeira linha do arquivo Csv.
     * Coloca csvFilePointer no primeiro caracetere da segunda linha
     * 
     * @param csvFilePointer Ponteiro aberto para o arquivo CSV
     * @param binFilePointer Ponteiro aberto para o arquivo binario de veiculos
    */

    veiculo_header *header = (veiculo_header *)calloc(sizeof(veiculo_header), 1 * sizeof(veiculo_header));
    alloc_check(header, "struct veiculo_header não foi alocado com sucesso\n");

    /* Colocar ponteiro no inicio do arquivo CSV */
    fseek(csvFilePointer, 0, SEEK_SET);

    /* Ler header do CSV */
    fscanf(csvFilePointer, "%[^,]%*c%[^,]%*c%[^,]%*c%[^,]%*c%[^,]%*c%[^\n]%*c", header->descrevePrefixo, 
        header->descreveData,
        header->descreveLugares,
        header->descreveLinha,
        header->descreveModelo,
        header->descreveCategoria);
    
    header->status = '0';                           /* Setar campo para arquivo "sujo" */
    header->byteProxReg = VEICULO_HEADER_SIZE + 1;  /* Setar o campo */

    #ifdef DEBUG
    print_header(header); 
    #endif

    /* Escrevendo informacoes lidas no arquivo binario */
    update_binary_veiculo_header(header, binFilePointer);

    /* Retorno de uma struct_header contendo o arquivo lido */
    return header;
}

void update_binary_veiculo_header(veiculo_header *header, FILE *binFilePointer)
{
    /**
     * Funcao para reescrever todo o cabecalho escrito no arquivo
     * Seta cursor no inicio do arquivo para a leitura e depois posiciona o cursor no 1o byte do 1o registro
     * 
     * @param header Struct armazenando o header do arquivo. Usado para escrever as informacoes salvas no header no arquivo
     * @param binFilePointer Ponteiro aberto para o arquivo binario do veiculo, usado para armazenar as informações dos registros.
    */

    /* Posicionar o cursor no inicio do arquivo binario para atualizar o aquivo binario */
    fseek(binFilePointer, 0, SEEK_SET);

    fwrite(&header->status, sizeof(char), 1, binFilePointer);
    fwrite(&header->byteProxReg, sizeof(long long int), 1, binFilePointer);
    fwrite(&header->nroRegistros, sizeof(int), 1, binFilePointer);
    fwrite(&header->nroRegRemovidos, sizeof(int), 1, binFilePointer);
    fwrite(header->descrevePrefixo, sizeof(char), 18, binFilePointer);
    fwrite(header->descreveData, sizeof(char), 35, binFilePointer);
    fwrite(header->descreveLugares, sizeof(char), 42, binFilePointer);
    fwrite(header->descreveLinha, sizeof(char), 26, binFilePointer);
    fwrite(header->descreveModelo, sizeof(char), 17, binFilePointer);
    fwrite(header->descreveCategoria, sizeof(char), 20, binFilePointer);

    return;
}

veiculo_data *read_csv_veiculo_data(veiculo_header *header, FILE *csvFilePointer, FILE *binFilePointer)
{
    /**
     * Funcao para ler todo o arquivo csv e passar para o arquivo binario no formato especificado
     * 
     * @param header
     * @param csvFilePointer Ponteiro aberto para o arquivo CSV
     * @param binFilePointer Ponteiro aberto para o arquivo binario do veiculo
    */

    veiculo_data *data = (veiculo_data *)calloc(sizeof(veiculo_data), 1 * sizeof(veiculo_data));
    alloc_check(data, "struct veiculo_data não alocado com sucesso\n");

    char tempRemovido;
    char tempQuantidadeLugares[64];
    char tempCodLinha[64];

    /* Posicionar cursor no byte depois do cabecalho */
    fseek(binFilePointer, VEICULO_HEADER_SIZE + 1, SEEK_SET);

    while((tempRemovido = getc(csvFilePointer)) != EOF) {

        /* Checa se o registro esta removido */
        if (tempRemovido != '*') {
            data->removido = '1';
            fseek(csvFilePointer, -1, SEEK_CUR);
            header->nroRegistros += 1;
        } else {
            data->removido = '0';
            header->nroRegRemovidos += 1;
        }

        /* Le todos os campos da linha como strings*/
        fscanf(csvFilePointer, "%[^,]%*c%[^,]%*c%[^,]%*c%[^,]%*c%[^,]%*c%[^\n]%*c", data->prefixo, 
            data->data,
            tempQuantidadeLugares,
            tempCodLinha,
            data->modelo,
            data->categoria);

        /* Checa por campos com valores "NULO" */
        if (is_null_field(data->data)) {
            memcpy(data->data, "\0@@@@@@@@@", 10);
        }

        if (is_null_field(data->modelo)) {
            memcpy(data->modelo, "\0", 1);
        }
        data->tamanhoModelo = strlen(data->modelo);

        if (is_null_field(data->categoria)) {
            memcpy(data->modelo, "\0", 1);
        }
        data->tamanhoCategoria = strlen(data->categoria);

        /* Lida com campos de valor inteiro */
        if (is_null_field(tempQuantidadeLugares)) {
            data->quantidadeLugares = -1;
        } else {
            data->quantidadeLugares = atoi(tempQuantidadeLugares);
        }

        if(is_null_field(tempCodLinha)) {
            data->codLinha = -1;
        } else {
            data->codLinha = atoi(tempCodLinha);
        }

        data->tamanhoRegistro = VEICULO_DATA_DEFAULT_SIZE + data->tamanhoModelo + data->tamanhoCategoria;

        #ifdef DEBUG
        print_data(data);
        #endif

        /* Escreve informação no arquivo binario */
        append_binary_veiculo_data (header, data, binFilePointer);
    }
    
    /* Retorna a struct data com as ultimas informacoes armazenadas */
    return data;
}

void append_binary_veiculo_data (veiculo_header *header, veiculo_data *data, FILE *binFilePointer)
{
    /* Posicionar o ponteiro na posicao do próximo registro */
    fseek(binFilePointer, header->byteProxReg, SEEK_SET);

    fwrite(&data->removido, sizeof(char), 1, binFilePointer);
    fwrite(&data->tamanhoRegistro, sizeof(int), 1, binFilePointer);
    fwrite(data->prefixo, sizeof(char), 5, binFilePointer);
    fwrite(data->data, sizeof(char), 10, binFilePointer);
    fwrite(&data->quantidadeLugares, sizeof(int), 1, binFilePointer);
    fwrite(&data->codLinha, sizeof(int), 1, binFilePointer);
    fwrite(&data->tamanhoModelo, sizeof(int), 1, binFilePointer);
    fwrite(data->modelo, sizeof(char), data->tamanhoModelo, binFilePointer);
    fwrite(&data->tamanhoCategoria, sizeof(int), 1, binFilePointer);
    fwrite(data->categoria, sizeof(char), data->tamanhoCategoria, binFilePointer);

    /* Colocar o offset para o próximo registro na posicao correta baseada no ultimo registro */
    header->byteProxReg = header->byteProxReg + data->tamanhoRegistro + 5;

    return;
}

void close_binary_veiculo_file(veiculo_file* file)
{
    /* Colocar o status do arquivo como "limpo" e atualizar o cabecalho do arquivo antes de fechar */
    file->header->status = '1';
    update_binary_veiculo_header(file->header, file->fp);

    #ifdef DEBUG
    print_header(file->header);
    #endif

    
    fclose(file->fp);
    free(file->header);
    free(file->data);
    free(file);
}