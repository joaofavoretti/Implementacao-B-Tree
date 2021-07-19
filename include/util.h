/**
 * Trabalho Prático 3 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#ifndef INCLUDE_UTIL_H
#define INCLUDE_UTIL_H

#define ALLOC_NULL_POINTER 0
#define FAIL_BINARY_STATUS 0

#include <io/write_veiculo.h>
#include <io/write_linha.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void alloc_check(void *data_pointer, char *message);
int is_null_field(char *field);
int cmp_veiculo(const void *a, const void *b);
int cmp_linha(const void *a, const void *b);
void generate_ordered_veiculo_file (FILE *unorderedFile, FILE *orderedFile);
void generate_ordered_linha_file (FILE *unorderedFile, FILE *orderedFile);

/* Funcoes fornecidas */
void binarioNaTela(char *nomeArquivoBinario);
void scan_quote_string(char *str);
int convertePrefixo(char* str);

#endif  /* util.h included */
