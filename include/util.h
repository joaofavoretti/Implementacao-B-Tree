/**
 * Trabalho Prático 2 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#ifndef _UTIL_H_
#define _UTIL_H_

#define ALLOC_NULL_POINTER 0
#define FAIL_BINARY_STATUS 0

#include <util.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void alloc_check(void *data_pointer, char *message);
int is_null_field(char *field);

/* Funcoes fornecidas */
void binarioNaTela(char *nomeArquivoBinario);
void scan_quote_string(char *str);
int convertePrefixo(char* str);


#endif