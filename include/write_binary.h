/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#ifndef _APPEND_BINARY_H_
#define _APPEND_BINARY_H_

#include <veiculo_read.h>
#include <linha_read.h>

void update_binary_veiculo_header(veiculo_header *header, FILE *binFilePointer);
void append_binary_veiculo_data (veiculo_header *header, veiculo_data *data, FILE *binFilePointer);
void update_binary_linha_header(linha_header *header, FILE *binFilePointer);
void append_binary_linha_data (linha_header *header, linha_data *data, FILE *binFilePointer);

#endif
