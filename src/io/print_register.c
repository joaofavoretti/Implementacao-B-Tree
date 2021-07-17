/**
 * Trabalho Prático 3 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <io/print_register.h>

char *format_data_field(char *data) 
{
    /**
     * Funcao para formatar string de data no formato AAAA-MM-DD
     * Retorna a string no formato especificado;
    */
    char meses[][16] = {"janeiro", "fevereiro", "março", "abril", "maio", "junho", "julho", "agosto", "setembro", "outubro", "novembro", "dezembro"};

    char *dataFormatada = (char *)calloc(64, sizeof(char));
    char dataMes[8] = {0};

    /* Calcula o indice o mes para o vetor meses. Considera que a representacao do mes esteja entre '1' e '12' */
    strncpy(dataMes, &data[5], 2);
    int numeroMes = atoi(dataMes) - 1;

    /* Concatena as informacoes da data */
    strncpy(dataFormatada, &data[8], 2);
    strncpy(&dataFormatada[2], " de ", 4);
    strncpy(&dataFormatada[6], meses[numeroMes], strlen(meses[numeroMes]));
    strncpy(&dataFormatada[strlen(dataFormatada)], " de ", 4);
    strncpy(&dataFormatada[strlen(dataFormatada)], &data[0], 4);

    return dataFormatada;
}


// Imprime um campo do registro
void print_field(char *descriptor, char *value)
{
    printf("%s: ", descriptor);

    if(strlen(value) == 0)
        printf("campo com valor nulo\n");
    else
        printf("%s\n", value);
}


// Imprime o registro veiculo
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


// Imprime o registro linha
void print_linha_data(linha_header *header, linha_data *data)
{
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


// Compara o valor 'fieldValue' com o valor do campo 'fieldName' 
int fieldcmp_veiculo(char *fieldValue, char *fieldName, veiculo_data *data)
{
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


// Compara o valor 'fieldValue' com o valor do campo 'fieldName' 
int fieldcmp_linha(char *fieldValue, char *fieldName, linha_data *data)
{
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


