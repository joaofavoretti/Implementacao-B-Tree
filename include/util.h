/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <veiculo_read.h>
#include <linha_read.h>
#include <stdlib.h>

#ifndef _UTIL_H_
#define _UTIL_H_

void print__veiculo_header(veiculo_header *header);
void print__veiculo_data(veiculo_data *data);
void print__linha_header(linha_header *header);
void print__linha_data(linha_data *data);
char *generate_bin_filename(char *fileName);

void print_veiculo_data(veiculo_header *header, veiculo_data *data);
void print_linha_data(linha_header *header, linha_data *data);
int fieldcmp_veiculo(char *fieldValue, char *fieldName, veiculo_data *data);
int fieldcmp_linha(char *fieldValue, char *fieldName, linha_data *data);

#endif