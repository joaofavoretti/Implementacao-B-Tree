/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <util.h>
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
    print_field(header->descreveData, data->data);
    char str[11];
    sprintf(str, "%d", data->quantidadeLugares);
    print_field(header->descreveLugares, str);
}


void print_linha_data(linha_header *header, linha_data *data){
    printf("%s: %d\n", header->descreveCodigo, data->codLinha);
    print_field(header->descreveNome, data->nomeLinha);
    print_field(header->descreveCor, data->corLinha);

    printf("%s: ", header->descreveCor);
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

