/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <veiculo_read.h>
#include <comandos.h>
#include <util.h>
#include <main.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void comando_1()
{
    /**
     * Comando 1 usado para ler um arquivo csv e gerar um arquivo binario
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