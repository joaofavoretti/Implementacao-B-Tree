/**
 * Trabalho Prático 2 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <io/read_linha.h>
#include <util.h>

linha_header *read_csv_linha_header(FILE *csvFilePointer)
{
    /**
     * Funcao para ler o cabecalho do arquivo csv.
     * 
     * @param csvFilePointer Ponteiro aberto para o arquivo CSV
     * @return ponteiro para struct linha_header com o dados do cabecalho
    */

    linha_header *header = (linha_header *)calloc(1, sizeof(linha_header));
    alloc_check(header, "struct linha_header não foi alocado com sucesso\n");

    /* Colocar ponteiro no inicio do arquivo CSV */
    fseek(csvFilePointer, 0, SEEK_SET);

    /* Ler header do CSV */
    fscanf(csvFilePointer, "%[^,]%*c%[^,]%*c%[^,]%*c%[^\n]%*c", header->descreveCodigo, 
        header->descreveCartao,
        header->descreveNome,
        header->descreveCor);
    
    header->status = '0';                     /* Setar campo para arquivo "sujo" */
    header->byteProxReg = LINHA_HEADER_SIZE;  /* Setar o inicio do proximo registro */

    /* Retorno de uma struct linha_header contendo o arquivo lido */
    return header;
}


linha_data *read_csv_linha_data(linha_header *header, FILE *csvFilePointer)
{
    /**
     * Funcao que permite ler uma linha(registro) do arquivo CSV
     * 
     * @param csvFilePointer - ponteiro aberto para o arquivo CSV
     * @return ponteiro para struct linha_data com os dados do registro
    */

    linha_data *data = (linha_data *)calloc(1, sizeof(linha_data));
    alloc_check(data, "struct linha_data não alocado com sucesso\n");

    char tempRemovido;
    char tempCodLinha[64];
    char tempAceitaCartao[64];

    /* Verifica se eh o final do arquivo */
    if((tempRemovido = getc(csvFilePointer)) == EOF){
        free(data);
        return NULL;
    }

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
    fscanf(csvFilePointer, "%[^,]%*c%[^,]%*c%[^,]%*c%[^\n]%*c",
        tempCodLinha, 
        tempAceitaCartao,
        data->nomeLinha,
        data->corLinha);

    /* Checa por campos com valores "NULO" */
    data->aceitaCartao = tempAceitaCartao[0];
    if (is_null_field(tempAceitaCartao))
        data->aceitaCartao = '\0';

    if (is_null_field(data->nomeLinha))
        memcpy(data->nomeLinha, "\0", 1);
    data->tamanhoNome = strlen(data->nomeLinha);

    if (is_null_field(data->corLinha))
        memcpy(data->corLinha, "\0", 1);
    data->tamanhoCor = strlen(data->corLinha);

    /* Lida com campos de valor inteiro */
    data->codLinha = atoi(tempCodLinha);
    data->tamanhoRegistro = LINHA_DATA_DEFAULT_SIZE + data->tamanhoNome + data->tamanhoCor;

    /* Retorna a struct cos os dados do registro */
    return data;
}


linha_header *read_binary_linha_header(FILE *binFilePointer)
{
    /**
     * Funcao para ler o header do binario fornecido por parametro.
     * Posiciona o cursor na posicao 0 do arquivo para realizar a leitura. 
     * Apos a leitura, posiciona o cursor no primeiro byte do primeiro registro.
     * 
     * @param binFilePointer Ponteiro aberto para o arquivo binario do veiculo
     */

    linha_header *header = (linha_header *)calloc(1, sizeof(linha_header));
    alloc_check(header, "struct linha_header não foi alocado com sucesso\n");

    /* Posiciona o cursor no comeco do arquivo */
    fseek(binFilePointer, 0, SEEK_SET);

    /* Le status */
    fread(&header->status, sizeof(char), 1, binFilePointer);

    /* Verifica se o arquivo eh inconsistente */
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


linha_data *read_binary_linha_data(FILE *binFilePointer)
{
    /**
     *  Função para ler um registro do binario fornecido por parametro
     *  @return Retorna uma struct linha_data com os dados do registro
     */

    linha_data *data = (linha_data *)calloc(1, sizeof(linha_data));
    alloc_check(data, "struct linha_data não foi alocado com sucesso\n");
    
    fread(&data->removido, sizeof(char), 1, binFilePointer);
    fread(&data->tamanhoRegistro, sizeof(int), 1, binFilePointer);

    //Verifica se o arquivo eh inconsistente
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


linha_data *read_stdin_linha_data(linha_header *header)
{
    /**
     * Funcao para ler nOfEntries da entrada padrao e escrever no lugar
     *                  do proximo registro do arquivo binario de linha
     * 
     * @param nOfEntries Numero de entradas que vao ser colocadas na entrada padrao
     * @param header Estrutura armazenando o cabecalho do arquivo binario do linha
     * @return Retorna uma struct linha_data com os dados do registro
    */

    linha_data *data = (linha_data *)calloc(1, sizeof(linha_data));
    alloc_check(data, "struct linha_data não alocado com sucesso\n");

    char tempCodLinha[64];
    char tempAceitaCartao[64];

    data->removido = '1';       /* Setar o registro como não removido */
    header->nroRegistros += 1;  /* Incrementar o numero de registros do arquivo */

    /* Le as entradas do usuario como string */
    /* A funcao scan_quote_string retorna o valor "" quando encontra NULO */
    scanf("%s ", tempCodLinha);
    scan_quote_string(tempAceitaCartao);
    scan_quote_string(data->nomeLinha);
    scan_quote_string(data->corLinha);

    /* Checa por campos com valores "NULO" */
    /* Como a funcao scan_quote_string ja retorna o campo como "" caso seja nulo,
                                     não precisa checar a string nesses campos */
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

    return data;
}