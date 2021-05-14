/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <veiculo_read.h>

#ifndef _UTIL_H_
#define _UTIL_H_

void print_header(veiculo_header *header);
void print_data(veiculo_data *data);
char *generate_bin_filename(char *fileName);

#endif