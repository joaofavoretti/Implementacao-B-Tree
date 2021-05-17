/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <veiculo_read.h>
#include <linha_read.h>
#include <write_binary.h>
#include <read_binary.h>
#include <close_binary.h>
#include <alloc_check.h>
#include <comandos.h>
#include <util.h>
#include <main.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void comando_1()
{
    /**
     * Comando 1 usado para ler um arquivo csv veiculo e gerar um arquivo binario veiculo
    */
    char *fileName = (char *)calloc(sizeof(char), 128 * sizeof(char));
    scanf("%s", fileName);

    char *binaryFileName = generate_bin_filename(fileName);

    veiculo_file *file = read_csv_veiculo_file(fileName, binaryFileName);
    close_binary_veiculo_file(file);
    binarioNaTela(binaryFileName);

    free(fileName);
    free(binaryFileName);
}

void comando_2()
{
    /**
     * Comando 2 usado para ler um arquivo csv linha e gerar um arquivo binario linha
    */

    char *fileName = (char *)calloc(sizeof(char), 128 * sizeof(char));
    scanf("%s", fileName);

    char *binaryFileName = generate_bin_filename(fileName);

    linha_file *file = read_csv_linha_file(fileName, binaryFileName);
    close_binary_linha_file(file);
    binarioNaTela(binaryFileName);

    free(fileName);
    free(binaryFileName);
}

void comando_3()
{
    /*
     *  Comando 3 usado para mostrar os dados de forma organizada dos registros em 'veiculo.bin'
     */

    char *binFileName = (char *)calloc(sizeof(char), 128 * sizeof(char));
    scanf("%s", binFileName);

    veiculo_file *file = (veiculo_file *)calloc(sizeof(veiculo_file), 1 * sizeof(veiculo_file));
    alloc_check(file, "Falha no processamento do arquivo.\n");

    file->fp = fopen(binFileName, "r");
    alloc_check(file->fp, "Falha no processamento do arquivo.\n");

    file->header = read_binary_veiculo_header(file->fp);

    for(int i = 0; i < file->header->nroRegistros; i++)
    {
        veiculo_data *data = read_binary_veiculo_data(file->header, file->fp);
        if(data->removido == '1') {
            print_veiculo_data(file->header, data);
            printf("\n");
        } else {
            fseek(file->fp, data->tamanhoRegistro, SEEK_CUR);
        }
        free(data);
    }

    close_binary_veiculo_file(file);
    free(binFileName);
}

void comando_4(){
    /*
     *  Comando 4 usado para mostrar os dados de forma organizada dos registros em 'linha.bin'
     */

    char *binFileName = (char *)calloc(sizeof(char), 128 * sizeof(char));
    scanf("%s", binFileName);

    linha_file *file = (linha_file *)calloc(sizeof(linha_file), 1 * sizeof(linha_file));
    alloc_check(file, "Falha no processamento do arquivo.\n");

    file->fp = fopen(binFileName, "r");
    alloc_check(file->fp, "Falha no processamento do arquivo.\n");

    file->header = read_binary_linha_header(file->fp);

    for(int i = 0; i < file->header->nroRegistros; i++)
    {
        linha_data *data = read_binary_linha_data(file->header, file->fp);
        if(data->removido == '1') {
            print_linha_data(file->header, data);
            printf("\n");
        } else {
            fseek(file->fp, data->tamanhoRegistro, SEEK_CUR);
        }
        free(data);
    }

    close_binary_linha_file(file);
    free(binFileName);
}

void comando_7()
{
    /**
     * Comando 7 Usado para ler entradas da entrada padrao e escrever no arquivo binario de veiculo
    */

    int n;
    char *binFileName = (char *)calloc(sizeof(char), 128 * sizeof(char));
    scanf("%s %d", binFileName, &n);

    veiculo_file *file = (veiculo_file *)calloc(sizeof(veiculo_file), 1 * sizeof(veiculo_file));
    alloc_check(file, "Falha no processamento do arquivo.\n");

    file->fp = fopen(binFileName, "r+");
    alloc_check(file->fp, "Falha no processamento do arquivo.\n");

    file->header = read_binary_veiculo_header(file->fp);
    #ifdef DEBUG
    print__veiculo_header(file->header);
    #endif

    file->data = read_stdin_veiculo_data(n, file->header, file->fp);

    close_binary_veiculo_file(file);
    binarioNaTela(binFileName);

    free(binFileName);
}

void comando_8()
{
    /**
     * Comando 8 Usado para ler entradas da entrada padrao e escrever no arquivo binario de linha
    */

    int n;
    char *binFileName = (char *)calloc(sizeof(char), 128 * sizeof(char));
    scanf("%s %d", binFileName, &n);

    linha_file *file = (linha_file *)calloc(sizeof(linha_file), 1 * sizeof(linha_file));
    alloc_check(file, "Falha no processamento do arquivo.\n");

    file->fp = fopen(binFileName, "r+");
    alloc_check(file->fp, "Falha no processamento do arquivo.\n");

    file->header = read_binary_linha_header(file->fp);
    #ifdef DEBUG
    print__linha_header(file->header);
    #endif

    file->data = read_stdin_linha_data(n, file->header, file->fp);

    close_binary_linha_file(file);
    binarioNaTela(binFileName);

    free(binFileName);
}
