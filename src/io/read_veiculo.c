/**
 * Trabalho Prático 2 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <io/read_veiculo.h>
#include <io/write_veiculo.h>
#include <util.h>

veiculo_header *read_csv_veiculo_header(FILE *csvFilePointer)
{
    /**
     * Funcao para ler o cabecalho do arquivo csv.
     * 
     * @param csvFilePointer Ponteiro aberto para o arquivo CSV
     * @return ponteiro para struct veiculo_header com os dados cabecalho
    */

    veiculo_header *header = (veiculo_header *)calloc(1, sizeof(veiculo_header));
    alloc_check(header, "struct veiculo_header não foi alocado com sucesso\n");

    /* Colocar ponteiro no inicio do arquivo CSV */
    fseek(csvFilePointer, 0, SEEK_SET);

    /* Ler header do CSV */
    fscanf(csvFilePointer, "%[^,]%*c%[^,]%*c%[^,]%*c%[^,]%*c%[^,]%*c%[^\n]%*c", 
        header->descrevePrefixo, 
        header->descreveData,
        header->descreveLugares,
        header->descreveLinha,
        header->descreveModelo,
        header->descreveCategoria);
    
    header->status = '0';                       /* Setar campo para arquivo "sujo" */
    header->byteProxReg = VEICULO_HEADER_SIZE;  /* Setar o campo */

    /* Retorno de uma struct veiculo_header contendo o arquivo lido */
    return header;
}


veiculo_data *read_csv_veiculo_data(veiculo_header* header, FILE *csvFilePointer)
{
    /**
     * Funcao que permite ler uma linha(registro) do arquivo CSV
     * 
     * @param csvFilePointer - ponteiro aberto para o arquivo CSV
     * @return ponteiro para struct veiculo_data com o registro
    */

    veiculo_data *data = (veiculo_data *)calloc(1, sizeof(veiculo_data));
    alloc_check(data, "struct veiculo_data não alocado com sucesso\n");

    char tempRemovido;
    char tempQuantidadeLugares[64];
    char tempCodLinha[64];

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
    fscanf(csvFilePointer, "%[^,]%*c%[^,]%*c%[^,]%*c%[^,]%*c%[^,]%*c%[^\n]%*c", data->prefixo, 
        data->data,
        tempQuantidadeLugares,
        tempCodLinha,
        data->modelo,
        data->categoria);

    /* Checa por campos com valores "NULO" */
    if (is_null_field(data->data)) 
        memcpy(data->data, "\0@@@@@@@@@", 10);

    if (is_null_field(data->modelo))
        memcpy(data->modelo, "\0", 1);
    data->tamanhoModelo = strlen(data->modelo);

    if (is_null_field(data->categoria))
        memcpy(data->modelo, "\0", 1);
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
    
    /* Retorna a struct data com os dados do registro*/
    return data;
}


veiculo_header *read_binary_veiculo_header(FILE *binFilePointer)
{
    /**
     * Funcao para ler o header do binario fornecido por parametro.
     * Posiciona o cursor na posicao 0 do arquivo para realizar a leitura. 
     * Apos a leitura, posiciona o cursor no primeiro byte do primeiro registro.
     * 
     * @param binFilePointer Ponteiro aberto para o arquivo binario do veiculo
     */

    veiculo_header *header = (veiculo_header *)calloc(1, sizeof(veiculo_header));
    alloc_check(header, "struct veiculo_header não foi alocado com sucesso\n");

    /* Posiciona o cursor no comeco do arquivo */
    fseek(binFilePointer, 0, SEEK_SET);

    /* Le status */
    fread(&header->status, sizeof(char), 1, binFilePointer);

    //Verifica se o arquivo eh inconsistente
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

veiculo_data *read_binary_veiculo_data(FILE *binFilePointer)
{
    /*
     *  Função para ler um registro do binario fornecido por parametro
     *  Retorna uma struct com os dados do registro
     */

    veiculo_data *data = (veiculo_data *)calloc(1, sizeof(veiculo_data));
    alloc_check(data, "struct veiculo_data não foi alocado com sucesso\n");

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


veiculo_data *read_stdin_veiculo_data(veiculo_header *header)
{
    /**
     * Funcao para ler nOfEntries da entrada padrao e escrever no lugar do proximo registro do arquivo binario
     * 
     * @param nOfEntries Numero de entradas que vao ser colocadas na entrada padrao
     * @param header Estrutura armazenando o cabecalho do arquivo binario do veiculo
     * @return Retorna uma struct veiculo_data com os dados do registro
    */

    veiculo_data *data = (veiculo_data *)calloc(1, sizeof(veiculo_data));
    alloc_check(data, "struct veiculo_data não alocado com sucesso\n");

    char tempQuantidadeLugares[64];
    char tempCodLinha[64];

    data->removido = '1';       /* Setar o registro como não removido */
    header->nroRegistros += 1;  /* Incrementar o numero de registros do arquivo */

    /* Le as entradas do usuario como string */
    /* A funcao scan_quote_string retorna o valor "" quando encontra NULO */
    scan_quote_string(data->prefixo);
    scan_quote_string(data->data);
    scanf("%s ", tempQuantidadeLugares);
    scanf("%s ", tempCodLinha);
    scan_quote_string(data->modelo);
    scan_quote_string(data->categoria);

    /* Checa por campos com valores "NULO" */
    /* Como a funcao scan_quote_string ja retorna o campo como "" caso seja nulo,
                                       não precisa checar a string nesses campos */
    if (strlen(data->data) == 0) {
        memcpy(data->data, "\0@@@@@@@@@", 10);
    }

    data->tamanhoModelo = strlen(data->modelo);

    data->tamanhoCategoria = strlen(data->categoria);

    /* Lida com campos de valor inteiro */
    /* Precisamos checar porque não lemos esses campos com a funcao scan_quote_string */
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

    //Retorna o ponteiro para struct com os dados do registro
    return data;
}
