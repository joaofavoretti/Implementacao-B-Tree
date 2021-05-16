/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#ifndef _CLOSE_BINARY_H_
#define _CLOSE_BINARY_H_

#include <veiculo_read.h>
#include <linha_read.h>
#include <write_binary.h>

void close_binary_linha_file(linha_file* file);
void close_binary_veiculo_file(veiculo_file* file);


#endif