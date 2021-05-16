/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <close_binary.h>
#include <util.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void close_binary_veiculo_file(veiculo_file* file)
{
    /**
     * Funcao usada para fechar o arquivo binario do veiculo. 
     * Atualiza o status do header para '1'
     * Atualiza o header com as informacoes que foram adicionadas durante o programa
     * 
     * @param file estrutura do arquivo de veiculo salva em RAM que deve ser fechada
     */

    /* Colocar o status do arquivo como "limpo" e atualizar o cabecalho do arquivo antes de fechar */
    file->header->status = '1';
    update_binary_veiculo_header(file->header, file->fp);

    #ifdef DEBUG
    print__veiculo_header(file->header);
    #endif

    fclose(file->fp);
    free(file->header);
    free(file->data);
    free(file);
}

void close_binary_linha_file(linha_file* file)
{
    /**
     * Funcao usada para fechar o arquivo binario do linha. 
     * Atualiza o status do header para '1'
     * Atualiza o header com as informacoes que foram adicionadas durante o programa
     * 
     * @param file estrutura do arquivo de linha salva em RAM que deve ser fechada
     */

    /* Colocar o status do arquivo como "limpo" e atualizar o cabecalho do arquivo antes de fechar */
    file->header->status = '1';
    update_binary_linha_header(file->header, file->fp);

    #ifdef DEBUG
    print__linha_header(file->header);
    #endif

    fclose(file->fp);
    free(file->header);
    free(file->data);
    free(file);
}