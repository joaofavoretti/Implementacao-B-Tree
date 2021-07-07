/**
 * Trabalho Prático 2 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#ifndef _WRITE_VEICULO_H_
#define _WRITE_VEICULO_H_

#include <read_veiculo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void update_binary_veiculo_header(veiculo_header *header, FILE *binFilePointer);
void append_binary_veiculo_data (veiculo_header *header, veiculo_data *data, FILE *binFilePointer);

#endif