/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <util.h>
#include <string.h>


void alloc_check(void *data_pointer, char *message)
{
    /**
     * Funcao geral para checar se pointeiro foi alocado
     * Caso não, o programa é encerrado com o código ALLOC_NULL_POINTER
    */
    if(data_pointer == NULL) {

        if (message != NULL) {
            printf("%s", message);
        }

        exit(ALLOC_NULL_POINTER);
    }
}


int is_null_field(char *field)
{
    /**
     * Checa se campo "field" tem a string "NULO", padrão usado no projeto para identificar um campo NULO
     * @return 1 se o campo tem o valor "NULO"
     * @return 0 se o campo possue um valor diferente de "NULO"
    */
    if (!strncmp(field, "NULO", 4)) {
        return 1;
    }
    return 0;
}
