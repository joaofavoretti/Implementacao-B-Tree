/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#ifndef _WRITE_LINHA_H_
#define _WRITE_LINHA_H_

#include <read_linha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void update_binary_linha_header(linha_header *header, FILE *binFilePointer);
void append_binary_linha_data (linha_header *header, linha_data *data, FILE *binFilePointer);

#endif