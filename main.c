/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <comandos.h>

int main(int argc, char **argv) {
    /* Recebe o comando */
    int comando;
    scanf("%d", &comando);

    /* Executa o comando */
    switch(comando) {
        case 1:
            /* CREATE TABLE - Lê veiculo.csv e grava os registros em arquivo binario */
            comando_1(); 
            break;

        case 2:
            /* CREATE TABLE - Lê linha.csv e grava os registros em arquivo binario */
            comando_2();
            break;
            
        case 3:
            /* SELECT - Recuperação dos registros de veiculo.bin */
            comando_3();
            break;
            
        case 4:
            /* SELECT - Recuperação dos registros de linha.bin */
            comando_4();
            break;
            
        case 5:
            /* SELECT WHERE - Recuperação dos registros de veiculo.bin com base no critério de busca */
            comando_5();
            break;
            
        case 6:
            /* SELECT WHERE - Recuperação dos registros de linha.bin com base no critério de busca */
            comando_6();
            break;
            
        case 7:
            /* INSERT INTO - Insere um novo registro, lido na entrada padrao, em veiculo.bin */
            comando_7();
            break;
            
        case 8:
            /* INSERT INTO - Insere um novo registro, lido na entrada padrao, em linha.bin */
            comando_8();
            break;

        case 9:
            comando_9();
            break;

        case 10:
            comando_10();
            break;

        case 11:
            comando_11();
            break;

        case 12:
            comando_12();
            break;
    }

    return 0;
}
