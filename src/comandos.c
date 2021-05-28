/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <read_veiculo.h>
#include <read_linha.h>
#include <write_veiculo.h>
#include <write_linha.h>
#include <print_register.h>
#include <util.h>
#include <comandos.h>
#include <main.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void comando_1()
{
    /**
     * Comando 1: usado para ler um arquivo csv veiculo e gerar um arquivo binario veiculo
    */
    // Recebe as entradas do comando
    char csvFileName[128], binFileName[128];
    scanf("%s %s", csvFileName, binFileName);

    // Abre arquivo csv para leitura
    FILE *csvFilePointer = fopen(csvFileName, "r");
    alloc_check(csvFilePointer, "Falha no processamento do arquivo.");

    // Abre arquivo binario para escrita
    FILE *binFilePointer = fopen(binFileName, "wb");

    // Le o cabecalho do csv e escreve no binario
    veiculo_header *header = read_csv_veiculo_header(csvFilePointer);
    update_binary_veiculo_header(header, binFilePointer);

    // Le linha por linha do csv e escreve no binario
    veiculo_data *data;
    while((data = read_csv_veiculo_data(header, csvFilePointer)) != NULL){
        append_binary_veiculo_data(header, data, binFilePointer);
        free(data);
    }

    // Coloca o arquivo como consistente e atualiza o header
    header->status = '1';
    update_binary_veiculo_header(header, binFilePointer);

    fclose(csvFilePointer);
    fclose(binFilePointer);
    free(header);
    binarioNaTela(binFileName);
}


void comando_2()
{
    /**
     * Comando 2: usado para ler um arquivo csv linha e gerar um arquivo binario linha
    */
    // Recebe as entradas do comando
    char csvFileName[128], binFileName[128];
    scanf("%s %s", csvFileName, binFileName);

    // Abre arquivo csv para leitura
    FILE *csvFilePointer = fopen(csvFileName, "r");
    alloc_check(csvFilePointer, "Falha no processamento do arquivo.");

    // Abre arquivo binario para escrita
    FILE *binFilePointer = fopen(binFileName, "wb");

    // Le o cabecalho do csv e escreve no binario
    linha_header *header = read_csv_linha_header(csvFilePointer);
    update_binary_linha_header(header, binFilePointer);

    // Le linha por linha do csv e escreve no binario
    linha_data *data;
    while((data = read_csv_linha_data(header, csvFilePointer)) != NULL){
        append_binary_linha_data(header, data, binFilePointer);
        free(data);
    }

    // Coloca o arquivo como consistente e atualiza o header
    header->status = '1';
    update_binary_linha_header(header, binFilePointer);

    fclose(csvFilePointer);
    fclose(binFilePointer);
    free(header);
    binarioNaTela(binFileName);
}


void comando_3()
{
    /*
     *  Comando 3: usado para mostrar os dados de forma organizada dos registros em 'veiculo.bin'
     */
    // Recebe as entradas do comando
    char binFileName[256];
    scanf("%s", binFileName);

    // Abre o arquivo binario para leitura
    FILE *binFilePointer = fopen(binFileName, "rb");
    alloc_check(binFilePointer, "Falha no processamento do arquivo.\n");

    // Le o cabecalho do arquivo binario
    veiculo_header *header = read_binary_veiculo_header(binFilePointer);

    int existeRegistro = 0;
    int totalDeRegistros = header->nroRegistros + header->nroRegRemovidos;

    // Imprime os registros que nao foram removidos
    for(int i = 0; i < totalDeRegistros; i++)
    {
        veiculo_data *data = read_binary_veiculo_data(binFilePointer);
        if(data->removido == '1') {
            existeRegistro = 1;
            print_veiculo_data(header, data);
            printf("\n");
        } else {
            fseek(binFilePointer, data->tamanhoRegistro, SEEK_CUR);
        }
        free(data);
    }

    // Informa se nenhum registro foi encontrado
    if (!existeRegistro)
        printf("Registro inexistente.\n");

    fclose(binFilePointer);
    free(header);
}


void comando_4(){
    /*
     *  Comando 4 usado para mostrar os dados de forma organizada dos registros em 'linha.bin'
     */
    // Recebe as entradas do comando
    char binFileName[256];
    scanf("%s", binFileName);

    // Abre o arquivo binario para leitura
    FILE *binFilePointer = fopen(binFileName, "rb");
    alloc_check(binFilePointer, "Falha no processamento do arquivo.\n");

    // Le o cabecalho do arquivo binario
    linha_header *header = read_binary_linha_header(binFilePointer);

    int existeRegistro = 0;
    int totalDeRegistros = header->nroRegistros + header->nroRegRemovidos;

    // Imprime os registros que nao foram removidos
    for(int i = 0; i < totalDeRegistros; i++)
    {
        linha_data *data = read_binary_linha_data(binFilePointer);
        if(data->removido == '1') {
            existeRegistro = 1;
            print_linha_data(header, data);
            printf("\n");
        } else {
            fseek(binFilePointer, data->tamanhoRegistro, SEEK_CUR);
        }
        free(data);
    }

    // Informa se nenhum registro foi encontrado
    if (!existeRegistro)
        printf("Registro inexistente.\n");

    fclose(binFilePointer);
    free(header);
}


void comando_5()
{
     /*
     *  Comando 5: usado para mostrar resgistros de 'veiculo.bin' com base no criterio de busca
     */
    // Recebe as entradas do comando
    char binFileName[128], fieldName[18], fieldValue[128];
    scanf("%s %s", binFileName, fieldName);
    scan_quote_string(fieldValue);

    // Abre arquivo binario para leitura
    FILE *binFilePointer = fopen(binFileName, "r");
    alloc_check(binFilePointer, "Falha no processamento do arquivo.\n");

    // Le o cabecalho do arquivo binario
    veiculo_header *header = read_binary_veiculo_header(binFilePointer);

    int existeRegistro = 0;
    int totalDeRegistros = header->nroRegistros + header->nroRegRemovidos;

    // Busca sequencial pelos registros do arquivo binario
    // Imprime aqueles que atendem o criterio de busca e nao foram removidos
    for(int i = 0; i < totalDeRegistros; i++)
    {
        veiculo_data *data = read_binary_veiculo_data(binFilePointer);

        if( data->removido == '1' && fieldcmp_veiculo(fieldValue, fieldName, data) == 0) {
            existeRegistro = 1;
            print_veiculo_data(header, data);
            printf("\n");

        } else if(data->removido == '0') {
            fseek(binFilePointer, data->tamanhoRegistro, SEEK_CUR);
        }

        free(data);
    }

    // Informa se nenhum registro foi encontrado
    if (!existeRegistro)
        printf("Registro inexistente.\n");

    fclose(binFilePointer);
    free(header);
}


void comando_6()
{
     /*
     *  Comando 6 usado para mostrar resgistros de 'linha.bin' com base no criterio de busca
     */
    // Recebe as entradas do comando
    char binFileName[128], fieldName[18], fieldValue[128];
    scanf("%s %s", binFileName, fieldName);
    scan_quote_string(fieldValue);

    // Abre arquivo binario para leitura
    FILE *binFilePointer = fopen(binFileName, "r");
    alloc_check(binFilePointer, "Falha no processamento do arquivo.\n");

    // Le o cabecalho do arquivo binario
    linha_header *header = read_binary_linha_header(binFilePointer);

    int existeRegistro = 0;
    int totalDeRegistros = header->nroRegistros + header->nroRegRemovidos;


    // Busca sequencial pelos registros do arquivo binario
    // Imprime aqueles que atendem o criterio de busca e nao foram removidos
    for(int i = 0; i < totalDeRegistros; i++)
    {
        linha_data *data = read_binary_linha_data(binFilePointer);

        if( data->removido == '1' && fieldcmp_linha(fieldValue, fieldName, data) == 0) {
            existeRegistro = 1;
            print_linha_data(header, data);
            printf("\n");

        } else if(data->removido == '0') {
            fseek(binFilePointer, data->tamanhoRegistro, SEEK_CUR);
        }

        free(data);
    }

    // Informa se nenhum registro foi encontrado
    if (!existeRegistro)
        printf("Registro inexistente.\n");

    fclose(binFilePointer);
    free(header);
}


void comando_7()
{
    /**
     * Comando 7: Usado para ler entradas da entrada padrao e escrever no arquivo binario de veiculo
    */

    // Recebe as entradas do comando
    int n;
    char binFileName[128];
    scanf("%s %d", binFileName, &n);

    // Abre arquivo binario para adicionar registros
    FILE *binFilePointer = fopen(binFileName, "r+");
    alloc_check(binFilePointer, "Falha no processamento do arquivo.\n");

    // Le o cabecalho do arquivo binario
    veiculo_header *header = read_binary_veiculo_header(binFilePointer);

    // Le veiculo por veiculo da entrada padrao (stdin) e escreve no binario
    veiculo_data *data;
    for(int i = 0; i < n; i++){
        data = read_stdin_veiculo_data(header);
        append_binary_veiculo_data(header, data, binFilePointer);
        free(data);
    }

    // Coloca o arquivo como consistente e atualiza o header
    header->status = '1';
    update_binary_veiculo_header(header, binFilePointer);

    fclose(binFilePointer);
    free(header);
    binarioNaTela(binFileName);
}


void comando_8()
{
    /**
     * Comando 8: Usado para ler entradas da entrada padrao e escrever no arquivo binario de linha
    */
   
    // Recebe as entradas do comando
    int n;
    char binFileName[128];
    scanf("%s %d", binFileName, &n);

    // Abre arquivo binario para adicionar registros
    FILE *binFilePointer = fopen(binFileName, "r+");
    alloc_check(binFilePointer, "Falha no processamento do arquivo.\n");

    // Le o cabecalho do arquivo binario
    linha_header *header = read_binary_linha_header(binFilePointer);

    // Le veiculo por veiculo da entrada padrao (stdin) e escreve no binario
    linha_data *data;
    for(int i = 0; i < n; i++){
        data = read_stdin_linha_data(header);
        append_binary_linha_data(header, data, binFilePointer);
        free(data);
    }

    // Coloca o arquivo como consistente e atualiza o header
    header->status = '1';
    update_binary_linha_header(header, binFilePointer);

    fclose(binFilePointer);
    free(header);
    binarioNaTela(binFileName);
}