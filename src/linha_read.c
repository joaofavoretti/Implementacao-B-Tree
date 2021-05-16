/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <linha_read.h>
#include <write_binary.h>
#include <close_binary.h>
#include <is_null_field.h>
#include <alloc_check.h>
#include <main.h>
#include <util.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

linha_file *read_csv_linha_file (char *csvFileName, char *binFileName)
{
    /**
     * Funcao principal para ler um arquivo CSV linha e passar para o arquivo binFileName.
     * 
     * @param csvFilename Nome do arquivo CSV do tipo linha que será lido
     * @param binFileName Nome do arquivo binario que as informacoes do arquivo CSV de linha
    */

    /* Abre o arquivo CSV */
    FILE *csvFilePointer = fopen (csvFileName, "r");
    alloc_check(csvFilePointer, "Falha no processamento do arquivo.\n");

    linha_file *file = (linha_file *)calloc(sizeof(linha_file), 1 * sizeof(linha_file));
    alloc_check(file, "struct linha_file não alocada com sucesso\n");

    file->fp = fopen(binFileName, "w+");
    alloc_check(csvFilePointer, "ponteiro para binario não foi alocado com sucesso\n");

    /* Leitura e escrita do cabecalho do csv */
    file->header = read_csv_linha_header(csvFilePointer, file->fp);

    /* Leitura e escrita das informacoes do csv */
    file->data = read_csv_linha_data(file->header, csvFilePointer, file->fp);

    fclose(csvFilePointer);
    return file;
}

linha_header *read_csv_linha_header(FILE *csvFilePointer, FILE *binFilePointer)
{
    /**
     * Funcao para ler a primeira linha do arquivo Csv.
     * Coloca csvFilePointer no primeiro caractere da segunda linha
     * 
     * @param csvFilePointer Ponteiro aberto para o arquivo CSV
     * @param binFilePointer Ponteiro aberto para o arquivo binario de linhas
    */

    linha_header *header = (linha_header *)calloc(sizeof(linha_header), 1 * sizeof(linha_header));
    alloc_check(header, "struct linha_header não foi alocado com sucesso\n");

    /* Colocar ponteiro no inicio do arquivo CSV */
    fseek(csvFilePointer, 0, SEEK_SET);

    /* Ler header do CSV */
    fscanf(csvFilePointer, "%[^,]%*c%[^,]%*c%[^,]%*c%[^\n]%*c", header->descreveCodigo, 
        header->descreveCartao,
        header->descreveNome,
        header->descreveCor);
    
    header->status = '0';                     /* Setar campo para arquivo "sujo" */
    header->byteProxReg = LINHA_HEADER_SIZE;  /* Setar o campo */

    #ifdef DEBUG
    print__linha_header(header);
    #endif

    /* Escrevendo informacoes lidas no arquivo binario */
    update_binary_linha_header(header, binFilePointer);

    /* Retorno de uma struct linha_header contendo o arquivo lido */
    return header;
}

linha_data *read_csv_linha_data(linha_header *header, FILE *csvFilePointer, FILE *binFilePointer)
{
    /**
     * Funcao para ler todo o arquivo csv e passar para o arquivo binario no formato especificado
     * 
     * @param header
     * @param csvFilePointer Ponteiro aberto para o arquivo CSV
     * @param binFilePointer Ponteiro aberto para o arquivo binario do linha
    */

    linha_data *data = (linha_data *)calloc(sizeof(linha_data), 1 * sizeof(linha_data));
    alloc_check(data, "struct linha_data não alocado com sucesso\n");

    char tempRemovido;
    char tempCodLinha[64];
    char tempAceitaCartao[64];

    /* Posicionar cursor no byte depois do cabecalho */
    fseek(binFilePointer, LINHA_HEADER_SIZE, SEEK_SET);

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
        fscanf(csvFilePointer, "%[^,]%*c%[^,]%*c%[^,]%*c%[^\n]%*c", tempCodLinha, 
            tempAceitaCartao,
            data->nomeLinha,
            data->corLinha);

        /* Checa por campos com valores "NULO" */
        if (is_null_field(tempAceitaCartao)) {
            memcpy(&data->aceitaCartao, "\0", 1);
        } else {
            memcpy(&data->aceitaCartao, tempAceitaCartao, 1);
        }

        if (is_null_field(data->nomeLinha)) {
            memcpy(data->nomeLinha, "\0", 1);
        }
        data->tamanhoNome = strlen(data->nomeLinha);

        if (is_null_field(data->corLinha)) {
            memcpy(data->corLinha, "\0", 1);
        }
        data->tamanhoCor = strlen(data->corLinha);

        /* Lida com campos de valor inteiro */
        data->codLinha = atoi(tempCodLinha); /* Caso especial do arquivo linha. codLinha não pode ser NULO */

        data->tamanhoRegistro = LINHA_DATA_DEFAULT_SIZE + data->tamanhoNome + data->tamanhoCor;

        #ifdef DEBUG
        print__linha_data(data);
        #endif

        /* Escreve informação no arquivo binario */
        append_binary_linha_data (header, data, binFilePointer);
    }
    
    /* Retorna a struct data com as ultimas informacoes armazenadas */
    return data;
}

linha_data *read_stdin_linha_data(int nOfEntries, linha_header *header, FILE *binFilePointer)
{
    /**
     * Funcao para ler nOfEntries da entrada padrao e escrever no lugar do proximo registro do arquivo binario de linha
     * 
     * @param nOfEntries Numero de entradas que vao ser colocadas na entrada padrao
     * @param header Estrutura armazenando o cabecalho do arquivo binario do linha
     * @param binFilePointer Ponteiro aberto para o arquivo binario de linha
    */

    linha_data *data = (linha_data *)calloc(sizeof(linha_data), 1 * sizeof(linha_data));
    alloc_check(data, "struct linha_data não alocado com sucesso\n");

    char tempCodLinha[64];
    char tempAceitaCartao[64];

    for (int i = 0; i < nOfEntries; i++) {
        data->removido = '1';       /* Setar o registro como não removido */
        header->nroRegistros += 1;  /* Incrementar o numero de registros do arquivo */

        /* Le as entradas do usuario como string */
        /* A funcao scan_quote_string retorna o valor "" quando encontra NULO */
        scanf("%s ", tempCodLinha);
        scan_quote_string(tempAceitaCartao);
        scan_quote_string(data->nomeLinha);
        scan_quote_string(data->corLinha);

        /* Checa por campos com valores "NULO" */
        /* Como a funcao scan_quote_string ja retorna o campo como "" caso seja nulo, não precisa checar a string nesses campos */
        if (strlen(tempAceitaCartao) == 0) {
            memcpy(&data->aceitaCartao, "\0", 1);
        } else {
            memcpy(&data->aceitaCartao, tempAceitaCartao, 1);
        }

        data->tamanhoNome = strlen(data->nomeLinha);

        data->tamanhoCor = strlen(data->corLinha);

        /* Lida com campos de valor inteiro */
        /* Precisamos checar porque não lemos esses campos com a funcao scan_quote_string */
        if(is_null_field(tempCodLinha)) {
            data->codLinha = -1;
        } else {
            data->codLinha = atoi(tempCodLinha);
        }

        data->tamanhoRegistro = LINHA_DATA_DEFAULT_SIZE + data->tamanhoNome + data->tamanhoCor;

        #ifdef DEBUG
        print__linha_data(data);
        #endif

        /* Escreve informação no arquivo binario */
        append_binary_linha_data (header, data, binFilePointer);
    }

    return data;
}