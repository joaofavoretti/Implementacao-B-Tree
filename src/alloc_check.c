/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <alloc_check.h>

/**
 * Funcao geral para checar se pointeiro foi alocado
 * Caso não, o programa é encerrado com o código ALLOC_NULL_POINTER
*/
void alloc_check(void *data_pointer, char *message)
{
    if(data_pointer == NULL) {

        if (message != NULL) {
            printf("%s", message);
        }

        exit(ALLOC_NULL_POINTER);
    }
}
