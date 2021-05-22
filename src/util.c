/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <util.h>
#include <alloc_check.h>
#include <stdlib.h>
#include <string.h>

char *generate_bin_filename(char *fileName)
{
    /**
     * Gera um nome para um arquivo binario para um arquivo csv
     * filename.csv -> filename.bin
     * @param fileName nome do arquivo csv com extensao .csv
    */

    /* Criar string de copia para não modificar o fileName */
    char *iterateString = (char *)calloc(sizeof(char), (strlen(fileName) + 1) * sizeof(char));
    strcpy (iterateString, fileName);

    char *binFilename = (char *)calloc(sizeof(char), (strlen(fileName) + 1) * sizeof(char));
    
    /* Criar o fileName para o arquivo binario */
    strcpy(binFilename, strtok(iterateString, "."));
    strcpy(&binFilename[strlen(binFilename)], ".bin");

    free(iterateString);

   return binFilename;
}

void print__veiculo_header(veiculo_header *header)
{
    /**
     * Funcão para debug. Imprimir struct veiculo_header com labels.
     * @param header struct alocada para ser impressa
    */
    printf("### VEICULO HEADER ###\n");
    printf("status: %c\n", header->status);
    printf("byteProxReg: %lld\n", header->byteProxReg);
    printf("nroRegistros: %d\n", header->nroRegistros);
    printf("nroRegRemovidos: %d\n", header->nroRegRemovidos);
    printf("descrevePrefixo: %s\n", header->descrevePrefixo);
    printf("descreveData: %s\n", header->descreveData);
    printf("descreveLugares: %s\n", header->descreveLugares);
    printf("descreveLinha: %s\n", header->descreveLinha);
    printf("descreveModelo: %s\n", header->descreveModelo);
    printf("descreveCategoria: %s\n", header->descreveCategoria);

    fflush(stdout);

    return;
}

void print__veiculo_data(veiculo_data *data)
{
    /**
     * Funcão para debug. Imprimir struct veiculo_data com labels.
     * @param data struct alocada para ser impressa
    */

    printf("### VEICULO DATA ###\n");
    printf("removido: %c\n", data->removido);
    printf("tamanhoRegistro: %d\n", data->tamanhoRegistro);
    printf("prefixo: %s\n", data->prefixo);
    printf("data: %.10s\n", data->data);
    printf("quantidadeLugares: %d\n", data->quantidadeLugares);
    printf("codLinha: %d\n", data->codLinha);
    printf("tamanhoModelo: %d\n", data->tamanhoModelo);
    printf("modelo: %s\n", data->modelo);
    printf("tamanhoCategoria: %d\n", data->tamanhoCategoria);
    printf("categoria: %s\n", data->categoria);

    fflush(stdout);

    return;
}

void print__linha_header(linha_header *header)
{
    /**
     * Funcão para debug. Imprimir struct linha_header com labels.
     * @param header struct alocada para ser impressa
    */
    printf("### LINHA HEADER ###\n");
    printf("status: %c\n", header->status);
    printf("byteProxReg: %lld\n", header->byteProxReg);
    printf("nroRegistros: %d\n", header->nroRegistros);
    printf("nroRegRemovidos: %d\n", header->nroRegRemovidos);
    printf("descreveCodigo: %s\n", header->descreveCodigo);
    printf("descreveCartao: %s\n", header->descreveCartao);
    printf("descreveNome: %s\n", header->descreveNome);
    printf("descreveCor: %s\n", header->descreveCor);

    fflush(stdout);

    return;
}

void print__linha_data(linha_data *data)
{
    /**
     * Funcão para debug. Imprimir struct linha_data com labels.
     * @param data struct alocada para ser impressa
    */

    printf("### LINHA DATA ###\n");
    printf("removido: %c\n", data->removido);
    printf("tamanhoRegistro: %d\n", data->tamanhoRegistro);
    printf("codLinha: %d\n", data->codLinha);
    printf("aceitaCartao: %c\n", data->aceitaCartao);
    printf("tamanhoNome: %d\n", data->tamanhoNome);
    printf("nomeLinha: %s\n", data->nomeLinha);
    printf("tamanhoCor: %d\n", data->tamanhoCor);
    printf("corLinha: %s\n", data->corLinha);

    fflush(stdout);

    return;
}

char *format_data_field(char *data) {
    /**
     * Funcao para formatar string de data no formato AAAA-MM-DD
     * retorna funcao alocada dinamicamente com a string formatada;
    */
    char meses[][16] = {"janeiro", "fevereiro", "março", "abril", "maio", "junho", "julho", "agosto", "setembro", "outubro", "novembro", "dezembro"};

    char *dataFormatada = (char *)calloc(sizeof(char), 64 * sizeof(char));
    alloc_check(dataFormatada, "Erro ao alocar memoria para dataFormatada");

    char *dataMes = (char *)calloc(sizeof(char), 8 * sizeof(char));
    alloc_check(dataMes, "Erro ao alocar memoria para dataMes");

    /* Calcula o indice o mes para o vetor meses. Considera que a representacao do mes esteja entre '1' e '12' */
    strncpy(dataMes, &data[5], 2);
    int numeroMes = atoi(dataMes) - 1;

    /* Concatena as informacoes para o formato ideal do mês */
    strncpy(dataFormatada, &data[8], 2);
    strncpy(&dataFormatada[2], " de ", 4);
    strncpy(&dataFormatada[6], meses[numeroMes], strlen(meses[numeroMes]));
    strncpy(&dataFormatada[strlen(dataFormatada)], " de ", 4);
    strncpy(&dataFormatada[strlen(dataFormatada)], &data[0], 4);

    free(dataMes);
    return dataFormatada;
}

void print_field(char *descriptor, char *value){
    printf("%s: ", descriptor);
    if(strlen(value) == 0)
        printf("campo com valor nulo\n");
    else
        printf("%s\n", value);
}

void print_veiculo_data(veiculo_header *header, veiculo_data *data)
{
    printf("%s: %s\n", header->descrevePrefixo, data->prefixo);
    print_field(header->descreveModelo, data->modelo);
    print_field(header->descreveCategoria, data->categoria);

    if (strlen(data->data)) {
        char *dataFormatada = format_data_field(data->data);
        print_field(header->descreveData, dataFormatada);
        free(dataFormatada);
    }  else {
        print_field(header->descreveData, data->data);
    }

    char str[11];
    sprintf(str, "%d", data->quantidadeLugares);
    print_field(header->descreveLugares, str);
}


void print_linha_data(linha_header *header, linha_data *data){
    printf("%s: %d\n", header->descreveCodigo, data->codLinha);
    print_field(header->descreveNome, data->nomeLinha);
    print_field(header->descreveCor, data->corLinha);

    printf("%s: ", header->descreveCartao);
    if(data->aceitaCartao == 'S'){
        printf("PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR\n");
    } else if(data->aceitaCartao == 'N') {
        printf("PAGAMENTO EM CARTAO E DINHEIRO\n");
    } else if(data->aceitaCartao == 'F') {
        printf("PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA\n");
    } else {
        printf("campo com valor nulo\n");
    }

}

int fieldcmp_veiculo(char *fieldValue, char *fieldName, veiculo_data *data){
    char *aux = NULL;

    if(strcmp("prefixo", fieldName) == 0) {
        aux = data->prefixo;
    } else if(strcmp("data", fieldName) == 0) {
        aux = data->data;
    } else if(strcmp("modelo", fieldName) == 0) {
        aux = data->modelo;
    } else if(strcmp("categoria", fieldName) == 0) {
        aux = data->categoria;
    } else if(strcmp("quantidadeLugares", fieldName) == 0) {
        return data->quantidadeLugares != atoi(fieldValue);
    }

    return strcmp(aux, fieldValue);
}

int fieldcmp_linha(char *fieldValue, char *fieldName, linha_data *data){
    char *aux = NULL;

    if(strcmp("aceitaCartao", fieldName) == 0) {
        return data->aceitaCartao != fieldValue[0];
    } else if(strcmp("nomeLinha", fieldName) == 0) {
        aux = data->nomeLinha;
    } else if(strcmp("corLinha", fieldName) == 0) {
        aux = data->corLinha;
    } else if(strcmp("codLinha", fieldName) == 0) {
        return data->codLinha != atoi(fieldValue);
    }

    return strcmp(aux, fieldValue);
}


