/**
 * Trabalho Prático 3 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#ifndef INCLUDE_IO_WRITE_LINHA_H
#define INCLUDE_IO_WRITE_LINHA_H

#include <io/read_linha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void update_binary_linha_header(linha_header *header, FILE *binFilePointer);
void append_binary_linha_data (linha_header *header, linha_data *data, FILE *binFilePointer);

#endif