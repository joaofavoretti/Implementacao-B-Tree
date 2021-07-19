/**
 * Trabalho Prático 3 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <io/write_linha.h>

void update_binary_linha_header(linha_header *header, FILE *binFilePointer)
{
    /**
     * Funcao para reescrever todo o cabecalho escrito no arquivo
     * Seta cursor no inicio do arquivo para a leitura e depois posiciona o cursor no 1o byte do 1o registro
     * 
     * @param header Struct armazenando o header do arquivo. Usado para escrever as informacoes salvas no header no arquivo
     * @param binFilePointer Ponteiro aberto para o arquivo binario do linha, usado para armazenar as informações dos registros.
    */

    /* Posicionar o cursor no inicio do arquivo binario para atualizar o aquivo binario */
    fseek(binFilePointer, 0, SEEK_SET);

    /* Reescreve os campos do cabecalho */
    fwrite(&header->status, sizeof(char), 1, binFilePointer);
    fwrite(&header->byteProxReg, sizeof(long long int), 1, binFilePointer);
    fwrite(&header->nroRegistros, sizeof(int), 1, binFilePointer);
    fwrite(&header->nroRegRemovidos, sizeof(int), 1, binFilePointer);
    fwrite(header->descreveCodigo, sizeof(char), 15, binFilePointer);
    fwrite(header->descreveCartao, sizeof(char), 13, binFilePointer);
    fwrite(header->descreveNome, sizeof(char), 13, binFilePointer);
    fwrite(header->descreveCor, sizeof(char), 24, binFilePointer);

    return;
}


void append_binary_linha_data (linha_header *header, linha_data *data, FILE *binFilePointer)
{
    /**
     * Funcao para adicionar um registro no arquivo binario linha.
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
    fwrite(&data->codLinha, sizeof(int), 1, binFilePointer);
    fwrite(&data->aceitaCartao, sizeof(char), 1, binFilePointer);
    fwrite(&data->tamanhoNome, sizeof(int), 1, binFilePointer);
    fwrite(data->nomeLinha, sizeof(char), data->tamanhoNome, binFilePointer);
    fwrite(&data->tamanhoCor, sizeof(int), 1, binFilePointer);
    fwrite(data->corLinha, sizeof(char), data->tamanhoCor, binFilePointer);

    /* Colocar o offset para o próximo registro na posicao correta baseada no ultimo registro */
    header->byteProxReg = header->byteProxReg + data->tamanhoRegistro + 5;

    return;
}