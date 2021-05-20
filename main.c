/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <main.h>
#include <comandos.h>

int main(int argc, char **argv) {
    int comando;
    scanf("%d", &comando);

    switch(comando) {
        case 1:
            comando_1();
            break;

        case 2:
            comando_2();
            break;
            
        case 3:
            comando_3();
            break;
            
        case 4:
            comando_4();
            break;
            
        case 5:
            break;
            
        case 6:
            break;
            
        case 7:
            comando_7();
            break;
            
        case 8:
            comando_8();
            break;
    }

    return 0;
}
