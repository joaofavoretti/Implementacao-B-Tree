/**
 * Trabalho Prático 3 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#ifndef INCLUDE_IO_WRITE_VEICULO_H
#define INCLUDE_IO_WRITE_VEICULO_H

#include <io/read_veiculo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void update_binary_veiculo_header(veiculo_header *header, FILE *binFilePointer);
void append_binary_veiculo_data (veiculo_header *header, veiculo_data *data, FILE *binFilePointer);

#endif