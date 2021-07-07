/**
 * Trabalho Prático 2 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <io/write_veiculo.h>

void update_binary_veiculo_header(veiculo_header *header, FILE *binFilePointer)
{
    /**
     * Funcao para reescrever todo o cabecalho escrito no arquivo
     * Seta cursor no inicio do arquivo para a leitura e depois posiciona o cursor no 1o byte do 1o registro
     * 
     * @param header Struct armazenando os dados do header do arquivo.
     * @param binFilePointer Ponteiro aberto para o arquivo binario do veiculo
    */

    /* Posicionar o cursor no inicio do arquivo binario para atualizar o aquivo binario */
    fseek(binFilePointer, 0, SEEK_SET);

    /* Reescreve os campos do cabecalho */
    fwrite(&header->status, sizeof(char), 1, binFilePointer);
    fwrite(&header->byteProxReg, sizeof(long long int), 1, binFilePointer);
    fwrite(&header->nroRegistros, sizeof(int), 1, binFilePointer);
    fwrite(&header->nroRegRemovidos, sizeof(int), 1, binFilePointer);
    fwrite(header->descrevePrefixo, sizeof(char), 18, binFilePointer);
    fwrite(header->descreveData, sizeof(char), 35, binFilePointer);
    fwrite(header->descreveLugares, sizeof(char), 42, binFilePointer);
    fwrite(header->descreveLinha, sizeof(char), 26, binFilePointer);
    fwrite(header->descreveModelo, sizeof(char), 17, binFilePointer);
    fwrite(header->descreveCategoria, sizeof(char), 20, binFilePointer);

    return;
}


void append_binary_veiculo_data (veiculo_header *header, veiculo_data *data, FILE *binFilePointer)
{
    /**
     * Funcao para adicionar um registro no arquivo binario veiculo.
     * 
     * @param header Struct armazenando os dados do header do arquivo.
     * @param data Struct armazenando os dados do registro que sera escrito.
     * @param binFilePointer Ponteiro aberto para o arquivo binario do veiculo
    */

    /* Posicionar o ponteiro na posicao do próximo registro */
    fseek(binFilePointer, header->byteProxReg, SEEK_SET);

    /* Escreve os campos do registro */
    fwrite(&data->removido, sizeof(char), 1, binFilePointer);
    fwrite(&data->tamanhoRegistro, sizeof(int), 1, binFilePointer);
    fwrite(data->prefixo, sizeof(char), 5, binFilePointer);
    fwrite(data->data, sizeof(char), 10, binFilePointer);
    fwrite(&data->quantidadeLugares, sizeof(int), 1, binFilePointer);
    fwrite(&data->codLinha, sizeof(int), 1, binFilePointer);
    fwrite(&data->tamanhoModelo, sizeof(int), 1, binFilePointer);
    fwrite(data->modelo, sizeof(char), data->tamanhoModelo, binFilePointer);
    fwrite(&data->tamanhoCategoria, sizeof(int), 1, binFilePointer);
    fwrite(data->categoria, sizeof(char), data->tamanhoCategoria, binFilePointer);

    /* Colocar o offset para o próximo registro na posicao correta baseada no ultimo registro */
    header->byteProxReg = header->byteProxReg + data->tamanhoRegistro + 5;

    return;
}