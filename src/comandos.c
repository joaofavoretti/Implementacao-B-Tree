/**
 * Trabalho Prático 1 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <comandos.h>

void comando_1()
{
    /**
     * Comando 1: usado para ler um arquivo csv veiculo e gerar um arquivo binario veiculo
    */
    // Recebe as entradas do comando
    char csvFileName[128], binFileName[128];
    scanf("%s %s", csvFileName, binFileName);

    // Abre arquivo csv para leitura
    FILE *csvFilePointer = fopen(csvFileName, "r");
    alloc_check(csvFilePointer, "Falha no processamento do arquivo.");

    // Abre arquivo binario para escrita
    FILE *binFilePointer = fopen(binFileName, "wb");

    // Le o cabecalho do csv e escreve no binario
    veiculo_header *header = read_csv_veiculo_header(csvFilePointer);
    update_binary_veiculo_header(header, binFilePointer);

    // Le linha por linha do csv e escreve no binario
    veiculo_data *data;
    while((data = read_csv_veiculo_data(header, csvFilePointer)) != NULL){
        append_binary_veiculo_data(header, data, binFilePointer);
        free(data);
    }

    // Coloca o arquivo como consistente e atualiza o header
    header->status = '1';
    update_binary_veiculo_header(header, binFilePointer);

    fclose(csvFilePointer);
    fclose(binFilePointer);
    free(header);
    binarioNaTela(binFileName);
}


void comando_2()
{
    /**
     * Comando 2: usado para ler um arquivo csv linha e gerar um arquivo binario linha
    */
    // Recebe as entradas do comando
    char csvFileName[128], binFileName[128];
    scanf("%s %s", csvFileName, binFileName);

    // Abre arquivo csv para leitura
    FILE *csvFilePointer = fopen(csvFileName, "r");
    alloc_check(csvFilePointer, "Falha no processamento do arquivo.");

    // Abre arquivo binario para escrita
    FILE *binFilePointer = fopen(binFileName, "wb");

    // Le o cabecalho do csv e escreve no binario
    linha_header *header = read_csv_linha_header(csvFilePointer);
    update_binary_linha_header(header, binFilePointer);

    // Le linha por linha do csv e escreve no binario
    linha_data *data;
    while((data = read_csv_linha_data(header, csvFilePointer)) != NULL){
        append_binary_linha_data(header, data, binFilePointer);
        free(data);
    }

    // Coloca o arquivo como consistente e atualiza o header
    header->status = '1';
    update_binary_linha_header(header, binFilePointer);

    fclose(csvFilePointer);
    fclose(binFilePointer);
    free(header);
    binarioNaTela(binFileName);
}


void comando_3()
{
    /*
     *  Comando 3: usado para mostrar os dados de forma organizada dos registros em 'veiculo.bin'
     */
    // Recebe as entradas do comando
    char binFileName[256];
    scanf("%s", binFileName);

    // Abre o arquivo binario para leitura
    FILE *binFilePointer = fopen(binFileName, "rb");
    alloc_check(binFilePointer, "Falha no processamento do arquivo.\n");

    // Le o cabecalho do arquivo binario
    veiculo_header *header = read_binary_veiculo_header(binFilePointer);

    int existeRegistro = 0;
    int totalDeRegistros = header->nroRegistros + header->nroRegRemovidos;

    // Imprime os registros que nao foram removidos
    for(int i = 0; i < totalDeRegistros; i++)
    {
        veiculo_data *data = read_binary_veiculo_data(binFilePointer);
        if(data->removido == '1') {
            existeRegistro = 1;
            print_veiculo_data(header, data);
            printf("\n");
        } else {
            fseek(binFilePointer, data->tamanhoRegistro, SEEK_CUR);
        }
        free(data);
    }

    // Informa se nenhum registro foi encontrado
    if (!existeRegistro)
        printf("Registro inexistente.\n");

    fclose(binFilePointer);
    free(header);
}


void comando_4(){
    /*
     *  Comando 4 usado para mostrar os dados de forma organizada dos registros em 'linha.bin'
     */
    // Recebe as entradas do comando
    char binFileName[256];
    scanf("%s", binFileName);

    // Abre o arquivo binario para leitura
    FILE *binFilePointer = fopen(binFileName, "rb");
    alloc_check(binFilePointer, "Falha no processamento do arquivo.\n");

    // Le o cabecalho do arquivo binario
    linha_header *header = read_binary_linha_header(binFilePointer);

    int existeRegistro = 0;
    int totalDeRegistros = header->nroRegistros + header->nroRegRemovidos;

    // Imprime os registros que nao foram removidos
    for(int i = 0; i < totalDeRegistros; i++)
    {
        linha_data *data = read_binary_linha_data(binFilePointer);
        if(data->removido == '1') {
            existeRegistro = 1;
            print_linha_data(header, data);
            printf("\n");
        } else {
            fseek(binFilePointer, data->tamanhoRegistro, SEEK_CUR);
        }
        free(data);
    }

    // Informa se nenhum registro foi encontrado
    if (!existeRegistro)
        printf("Registro inexistente.\n");

    fclose(binFilePointer);
    free(header);
}


void comando_5()
{
     /*
     *  Comando 5: usado para mostrar resgistros de 'veiculo.bin' com base no criterio de busca
     */
    // Recebe as entradas do comando
    char binFileName[128], fieldName[18], fieldValue[128];
    scanf("%s %s", binFileName, fieldName);
    scan_quote_string(fieldValue);

    // Abre arquivo binario para leitura
    FILE *binFilePointer = fopen(binFileName, "r");
    alloc_check(binFilePointer, "Falha no processamento do arquivo.\n");

    // Le o cabecalho do arquivo binario
    veiculo_header *header = read_binary_veiculo_header(binFilePointer);

    int existeRegistro = 0;
    int totalDeRegistros = header->nroRegistros + header->nroRegRemovidos;

    // Busca sequencial pelos registros do arquivo binario
    // Imprime aqueles que atendem o criterio de busca e nao foram removidos
    for(int i = 0; i < totalDeRegistros; i++)
    {
        veiculo_data *data = read_binary_veiculo_data(binFilePointer);

        if( data->removido == '1' && fieldcmp_veiculo(fieldValue, fieldName, data) == 0) {
            existeRegistro = 1;
            print_veiculo_data(header, data);
            printf("\n");

        } else if(data->removido == '0') {
            fseek(binFilePointer, data->tamanhoRegistro, SEEK_CUR);
        }

        free(data);
    }

    // Informa se nenhum registro foi encontrado
    if (!existeRegistro)
        printf("Registro inexistente.\n");

    fclose(binFilePointer);
    free(header);
}


void comando_6()
{
     /*
     *  Comando 6 usado para mostrar resgistros de 'linha.bin' com base no criterio de busca
     */
    // Recebe as entradas do comando
    char binFileName[128], fieldName[18], fieldValue[128];
    scanf("%s %s", binFileName, fieldName);
    scan_quote_string(fieldValue);

    // Abre arquivo binario para leitura
    FILE *binFilePointer = fopen(binFileName, "r");
    alloc_check(binFilePointer, "Falha no processamento do arquivo.\n");

    // Le o cabecalho do arquivo binario
    linha_header *header = read_binary_linha_header(binFilePointer);

    int existeRegistro = 0;
    int totalDeRegistros = header->nroRegistros + header->nroRegRemovidos;


    // Busca sequencial pelos registros do arquivo binario
    // Imprime aqueles que atendem o criterio de busca e nao foram removidos
    for(int i = 0; i < totalDeRegistros; i++)
    {
        linha_data *data = read_binary_linha_data(binFilePointer);

        if( data->removido == '1' && fieldcmp_linha(fieldValue, fieldName, data) == 0) {
            existeRegistro = 1;
            print_linha_data(header, data);
            printf("\n");

        } else if(data->removido == '0') {
            fseek(binFilePointer, data->tamanhoRegistro, SEEK_CUR);
        }

        free(data);
    }

    // Informa se nenhum registro foi encontrado
    if (!existeRegistro)
        printf("Registro inexistente.\n");

    fclose(binFilePointer);
    free(header);
}


void comando_7()
{
    /**
     * Comando 7: Usado para ler entradas da entrada padrao e escrever no arquivo binario de veiculo
    */

    // Recebe as entradas do comando
    int n;
    char binFileName[128];
    scanf("%s %d", binFileName, &n);

    // Abre arquivo binario para adicionar registros
    FILE *binFilePointer = fopen(binFileName, "r+");
    alloc_check(binFilePointer, "Falha no processamento do arquivo.\n");

    // Le o cabecalho do arquivo binario
    veiculo_header *header = read_binary_veiculo_header(binFilePointer);

    // Le veiculo por veiculo da entrada padrao (stdin) e escreve no binario
    veiculo_data *data;
    for(int i = 0; i < n; i++){
        data = read_stdin_veiculo_data(header);
        append_binary_veiculo_data(header, data, binFilePointer);
        free(data);
    }

    // Coloca o arquivo como consistente e atualiza o header
    header->status = '1';
    update_binary_veiculo_header(header, binFilePointer);

    fclose(binFilePointer);
    free(header);
    binarioNaTela(binFileName);
}


void comando_8()
{
    /**
     * Comando 8: Usado para ler entradas da entrada padrao e escrever no arquivo binario de linha
    */
   
    // Recebe as entradas do comando
    int n;
    char binFileName[128];
    scanf("%s %d", binFileName, &n);

    // Abre arquivo binario para adicionar registros
    FILE *binFilePointer = fopen(binFileName, "r+");
    alloc_check(binFilePointer, "Falha no processamento do arquivo.\n");

    // Le o cabecalho do arquivo binario
    linha_header *header = read_binary_linha_header(binFilePointer);

    // Le veiculo por veiculo da entrada padrao (stdin) e escreve no binario
    linha_data *data;
    for(int i = 0; i < n; i++){
        data = read_stdin_linha_data(header);
        append_binary_linha_data(header, data, binFilePointer);
        free(data);
    }

    // Coloca o arquivo como consistente e atualiza o header
    header->status = '1';
    update_binary_linha_header(header, binFilePointer);

    fclose(binFilePointer);
    free(header);
    binarioNaTela(binFileName);
}


void comando_9()
{
    /**
     *  Esse comando permitir criar um arquivo de indice (arvore-B)
     *  para o arquivo de dados veiculo 
     */
    // Recebe as entradas do comando
    char dataFilename[128], bTreeFilename[128];
    scanf("%s %s", dataFilename, bTreeFilename);

    // Abre o arquivo de dados para leitura e o de indice para escrita
    FILE *dataFile = fopen(dataFilename, "r");
    alloc_check(dataFile, "Falha no processamento do arquivo.\n");
    FILE *bTreeFile = fopen(bTreeFilename, "w+");
    alloc_check(bTreeFile, "Falha no processamento do arquivo.\n");

    // Le cabecalho do arquivo de dados
    veiculo_header *veiculoHeader = read_binary_veiculo_header(dataFile);
    // Inicializa o cabecalho do qruivo de indice com status inconsistente
    bTree_header *bTreeHeader = calloc(1, sizeof(bTree_header));
    bTreeHeader->status = '0';
    bTreeHeader->noRaiz = -1;
    update_bTree_header(bTreeHeader, bTreeFile);

    // Le registro por registro do arquivo de dados
    // e insere as chaves no arquivo de indice
    int totalDeRegistros = veiculoHeader->nroRegistros + veiculoHeader->nroRegRemovidos;
    for(int i = 0; i < totalDeRegistros; i++)
    {
        long long int reference = ftell(dataFile);
        veiculo_data *data = read_binary_veiculo_data(dataFile);
        // Verifica se nao esta logicamente removido
        if(data->removido == '1')
            insert(convertePrefixo(data->prefixo), reference, bTreeHeader, bTreeFile);
        else
            fseek(dataFile, data->tamanhoRegistro, SEEK_CUR);
        free(data);
    }

    // Muda o estado do arquivo para consistente e atualiza cabecalho
    bTreeHeader->status = '1';
    update_bTree_header(bTreeHeader, bTreeFile);

    
    free(veiculoHeader);
    free(bTreeHeader);
    fclose(dataFile);
    fclose(bTreeFile);
    binarioNaTela(bTreeFilename);
}


void comando_10()
{
    /**
     *  Esse comando permitir criar um arquivo de indice (arvore-B)
     *  para o arquivo de dados linha 
     */
    // Recebe as entradas do comando
    char dataFilename[128], bTreeFilename[128];
    scanf("%s %s", dataFilename, bTreeFilename);

    // Abre o arquivo de dados para leitura e o de indice para escrita
    FILE *dataFile = fopen(dataFilename, "r");
    alloc_check(dataFile, "Falha no processamento do arquivo.\n");
    FILE *bTreeFile = fopen(bTreeFilename, "w+");
    alloc_check(bTreeFile, "Falha no processamento do arquivo.\n");

    // Le cabecalho do arquivo de dados
    linha_header *linhaHeader = read_binary_linha_header(dataFile);
    // Inicializa o cabecalho do qruivo de indice com status inconsistente
    bTree_header *bTreeHeader = calloc(1, sizeof(bTree_header));
    bTreeHeader->status = '0';
    bTreeHeader->noRaiz = -1;
    update_bTree_header(bTreeHeader, bTreeFile);

    // Le registro por registro do arquivo de dados
    // e insere as chaves no arquivo de indice
    int totalDeRegistros = linhaHeader->nroRegistros + linhaHeader->nroRegRemovidos;
    for(int i = 0; i < totalDeRegistros; i++)
    {
        long long int reference = ftell(dataFile);
        linha_data *data = read_binary_linha_data(dataFile);
        // Verifica se nao esta logicamente removido
        if(data->removido == '1')
            insert(data->codLinha, reference, bTreeHeader, bTreeFile);
        else
            fseek(dataFile, data->tamanhoRegistro, SEEK_CUR);
        free(data);
    }

    // Muda o estado do arquivo para consistente e atualiza cabecalho
    bTreeHeader->status = '1';
    update_bTree_header(bTreeHeader, bTreeFile);

    
    free(linhaHeader);
    free(bTreeHeader);
    fclose(dataFile);
    fclose(bTreeFile);
    binarioNaTela(bTreeFilename);
}


void comando_11()
{
    /**
     * Comando usado para recuperar um registro do tipo veiculo
     *  pelo campo prefixo
     */

    // Recebe as entradas do comando
    char dataFilename[128], bTreeFilename[128], prefix[128];
    scanf("%s %s prefixo ", dataFilename, bTreeFilename);
    scan_quote_string(prefix);

    // Abre arquivos binario para leitura
    FILE *dataFile = fopen(dataFilename, "r");
    alloc_check(dataFile, "Falha no processamento do arquivo.\n");
    FILE *bTreeFile = fopen(bTreeFilename, "r");
    alloc_check(bTreeFile, "Falha no processamento do arquivo.\n");

    // Le cabecalhos
    veiculo_header *dataHeader = read_binary_veiculo_header(dataFile);
    bTree_header *bTreeHeader = read_bTree_header(bTreeFile);

    // Pesquisa pela chave no arquivo de indice e retorna a referencia
    int key = convertePrefixo(prefix);
    long long int ref;
    int res = search(bTreeHeader->noRaiz, key, bTreeFile, &ref);

    // Verifica se encontrou
    if(res == NOT_FOUND)
        printf("Registro inexistente.\n");
    else{
        // Acessa a posicao do registro no arquivo de dados
        fseek(dataFile, ref, SEEK_SET);
        // Le o registro
        veiculo_data *data = read_binary_veiculo_data(dataFile);
        // Imprime
        print_veiculo_data(dataHeader, data);
        printf("\n");
        free(data);
    }

    // Fecha arquivos e libera memoria heap
    fclose(dataFile);
    fclose(bTreeFile);
    free(dataHeader);
    free(bTreeHeader);
}

void comando_12()
{
    /**
     * Comando usado para recuperar um registro do tipo linha
     *  pelo campo prefixo
     */

    // Recebe as entradas do comando
    char dataFilename[128], bTreeFilename[128];
    int codLinha;
    scanf("%s %s codLinha %d", dataFilename, bTreeFilename, &codLinha);

    // Abre arquivos binario para leitura
    FILE *dataFile = fopen(dataFilename, "r");
    alloc_check(dataFile, "Falha no processamento do arquivo.\n");
    FILE *bTreeFile = fopen(bTreeFilename, "r");
    alloc_check(bTreeFile, "Falha no processamento do arquivo.\n");

    // Le cabecalhos
    linha_header *dataHeader = read_binary_linha_header(dataFile);
    bTree_header *bTreeHeader = read_bTree_header(bTreeFile);

    // Pesquisa pela chave no arquivo de indice e retorna a referencia
    long long int ref;
    int res = search(bTreeHeader->noRaiz, codLinha, bTreeFile, &ref);

    // Verifica se encontrou
    if(res == NOT_FOUND)
        printf("Registro inexistente.\n");
    else{
        // Acessa a posicao do registro no arquivo de dados
        fseek(dataFile, ref, SEEK_SET);
        // Le o registro
        linha_data *data = read_binary_linha_data(dataFile);
        // Imprime
        print_linha_data(dataHeader, data);
        printf("\n");
        free(data);
    }

    // Fecha arquivos e libera memoria heap
    fclose(dataFile);
    fclose(bTreeFile);
    free(dataHeader);
    free(bTreeHeader);
}


void comando_13()
{
    /**
     * Extensao do comando 7: Usado para ler entradas da entrada padrao e escrever no arquivo binario de veiculo, 
     * guardando as chaves e referencias em um arquivo de indice arcore-B
    */

    // Recebe as entradas do comando
    char dataFilename[128], bTreeFilename[128], prefix[128];
    int n;
    scanf("%s %s %d", dataFilename, bTreeFilename, &n);

    // Abre arquivos binario para leitura
    FILE *dataFile = fopen(dataFilename, "r+");
    alloc_check(dataFile, "Falha no processamento do arquivo.\n");
    FILE *bTreeFile = fopen(bTreeFilename, "r+");
    alloc_check(bTreeFile, "Falha no processamento do arquivo.\n");

    // Le cabecalhos
    veiculo_header *dataHeader = read_binary_veiculo_header(dataFile);
    bTree_header *bTreeHeader = read_bTree_header(bTreeFile);

    // Arquivos incosistenets
    dataHeader->status = '0';
    bTreeHeader->status = '0';
    update_binary_veiculo_header(dataHeader, dataFile);
    update_bTree_header(bTreeHeader, bTreeFile);

    // Le veiculo por veiculo da entrada padrao (stdin) e escreve no binario
    veiculo_data *data;
    for(int i = 0; i < n; i++){
        // INsere no arquivo de dados veiculo
        long long int reference = dataHeader->byteProxReg;
        data = read_stdin_veiculo_data(dataHeader);
        append_binary_veiculo_data(dataHeader, data, dataFile);
        // Insere no arquivo de indice
        insert(convertePrefixo(data->prefixo), reference, bTreeHeader, bTreeFile);
        free(data);
    }

    // Coloca oS arquivo como consistenteS e atualiza o header
    dataHeader->status = '1';
    bTreeHeader->status = '1';
    update_binary_veiculo_header(dataHeader, dataFile);
    update_bTree_header(bTreeHeader, bTreeFile);

    fclose(dataFile);
    fclose(bTreeFile);
    free(dataHeader);
    free(bTreeHeader);
    binarioNaTela(bTreeFilename);
}


void comando_14()
{
    /**
     * Extensao do comando 8: Usado para ler entradas da entrada padrao e escrever no arquivo binario de linha, 
     * guardando as chaves e referencias em um arquivo de indice arcore-B
    */

    // Recebe as entradas do comando
    char dataFilename[128], bTreeFilename[128], prefix[128];
    int n;
    scanf("%s %s %d", dataFilename, bTreeFilename, &n);

    // Abre arquivos binario para leitura
    FILE *dataFile = fopen(dataFilename, "r+");
    alloc_check(dataFile, "Falha no processamento do arquivo.\n");
    FILE *bTreeFile = fopen(bTreeFilename, "r+");
    alloc_check(bTreeFile, "Falha no processamento do arquivo.\n");

    // Le cabecalhos
    linha_header *dataHeader = read_binary_linha_header(dataFile);
    bTree_header *bTreeHeader = read_bTree_header(bTreeFile);

    // Arquivos incosistenets
    dataHeader->status = '0';
    bTreeHeader->status = '0';
    update_binary_linha_header(dataHeader, dataFile);
    update_bTree_header(bTreeHeader, bTreeFile);

    // Le linha por linha da entrada padrao (stdin) e escreve no binario
    linha_data *data;
    for(int i = 0; i < n; i++){
        // INsere no arquivo de dados linha
        long long int reference = dataHeader->byteProxReg;
        data = read_stdin_linha_data(dataHeader);
        append_binary_linha_data(dataHeader, data, dataFile);
        // Insere no arquivo de indice
        insert(data->codLinha, reference, bTreeHeader, bTreeFile);
        free(data);
    }

    // Coloca oS arquivo como consistenteS e atualiza o header
    dataHeader->status = '1';
    bTreeHeader->status = '1';
    update_binary_linha_header(dataHeader, dataFile);
    update_bTree_header(bTreeHeader, bTreeFile);

    fclose(dataFile);
    fclose(bTreeFile);
    free(dataHeader);
    free(bTreeHeader);
    binarioNaTela(bTreeFilename);
}