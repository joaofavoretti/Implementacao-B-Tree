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
    // Inicializa o cabecalho do qruivo de indice
    bTree_header *bTreeHeader = malloc(sizeof(bTree_header));
    bTreeHeader->noRaiz = 0;
    bTreeHeader->RRNproxNo = 1;
    bTreeHeader->status = '0';
    update_bTree_header(bTreeHeader, bTreeFile);

    //Inicializa o arquivo com o no raiz
    bTree_page raiz;
    raiz.folha = '1';
    raiz.RRNdoNo = 0;
    raiz.nroChavesIndexadas = 0;
    for(int i = 0; i < 4; i++){
        raiz.c[i] = -1;
        raiz.pr[i] = -1;
        raiz.p[i] = -1;
    } 
    raiz.p[4] = -1;

    write_bTree_page(0, &raiz, bTreeFile);


    // Le registro por registro do arquivo de dados
    // e insere as chaves no arquivo de indice
    int totalDeRegistros = veiculoHeader->nroRegistros + veiculoHeader->nroRegRemovidos;
    // printf("TOTAL: %d\n", totalDeRegistros);
    for(int i = 0; i < totalDeRegistros; i++)
    {
        // printf("\n#########################\n");
        veiculo_data *data = read_binary_veiculo_data(dataFile);
        if(data->removido == '1') {
            bTree_pair pair;
            int promotedRNN;
            pair.key = convertePrefixo(data->prefixo);
            pair.reference = ftell(dataFile);
            printf("Key: %d, Ref: %ld\n", pair.key, pair.reference);
            int res = insert(bTreeHeader->noRaiz, bTreeHeader, bTreeFile, &pair, &promotedRNN);
            if(res == PROMOTED){
                raiz.folha = '0';
                raiz.RRNdoNo = bTreeHeader->RRNproxNo;
                raiz.nroChavesIndexadas = 1;
                for(int i = 0; i < 4; i++){
                    raiz.c[i] = -1;
                    raiz.pr[i] = -1;
                    raiz.p[i] = -1;
                } 
                raiz.p[4] = -1;

                raiz.c[0] = pair.key;
                raiz.pr[0] = pair.reference;
                raiz.p[0] = bTreeHeader->noRaiz;
                raiz.p[1] = promotedRNN;

                bTreeHeader->noRaiz = raiz.RRNdoNo;
                bTreeHeader->RRNproxNo += 1;

                // printf("\n######\nnNova raiz\n:");
                // printf("RRN da raiz = %d\n", raiz.RRNdoNo);
                // for(int i = 0; i < 4; i++)
                //     printf("%d ", raiz.c[i]);
                // printf("\n");

                write_bTree_page(raiz.RRNdoNo, &raiz, bTreeFile);
            }
        } else {
            fseek(dataFile, data->tamanhoRegistro, SEEK_CUR);
        }
        free(data);
    }


    bTreeHeader->status = '1';
    update_bTree_header(bTreeHeader, bTreeFile);

    
    free(veiculoHeader);
    free(bTreeHeader);
    fclose(dataFile);
    fclose(bTreeFile);

    bTreeFile = fopen("casos_abertos/indicePrefixo1_my.bin", "r");
    bTreeHeader = read_bTree_header(bTreeFile);
    int i = 0;
    while(!feof(bTreeFile)){
        bTree_page *page = read_bTree_page(i, bTreeFile);
        print_bTree_page(page);
        free(page);
        i++;
        printf("\n\n");
    }

    fclose(bTreeFile);
    free(bTreeHeader);
    binarioNaTela(bTreeFilename);
}


void comando_10()
{

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
    long int ref;
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
    long int ref;
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
        free(data);
    }

    // Fecha arquivos e libera memoria heap
    fclose(dataFile);
    fclose(bTreeFile);
    free(dataHeader);
    free(bTreeHeader);
}