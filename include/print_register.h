/**
 * Trabalho Prático 2 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#ifndef _PRINT_REGISTER_
#define _PRINT_REGISTER_

#include <read_veiculo.h>
#include <read_linha.h>
#include <stdlib.h>
#include <string.h>

void print_veiculo_data(veiculo_header *header, veiculo_data *data);
int fieldcmp_veiculo(char *fieldValue, char *fieldName, veiculo_data *data);

void print_linha_data(linha_header *header, linha_data *data);
int fieldcmp_linha(char *fieldValue, char *fieldName, linha_data *data);

#endif