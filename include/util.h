/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#ifndef _UTIL_H_
#define _UTIL_H_

#define ALLOC_NULL_POINTER 0

#include <stdio.h>
#include <stdlib.h>

void alloc_check(void *data_pointer, char *message);
int is_null_field(char *field);

#endif