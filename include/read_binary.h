/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#ifndef _READ_BINARY_H_
#define _READ_BINARY_H_

#include <veiculo_read.h>
#include <linha_read.h>

veiculo_header *read_binary_veiculo_header(FILE *binFilePointer);
linha_header *read_binary_linha_header(FILE *binFilePointer);

#endif