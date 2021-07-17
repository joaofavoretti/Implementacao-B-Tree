/**
 * Trabalho Prático 2 - Organizacao de Arquivos
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
     *  Esse comando permite criar um arquivo de indice (arvore-B)
     *  para o arquivo de dados veiculo 
     */

    char dataFilename[128], BTreeFilename[128];         /* Recebe as primeiras duas entradas depois do comando  */
    scanf("%s %s", dataFilename, BTreeFilename);        /* ex entrada: 9 veiculo.bin indicePrefixo.bin          */

    FILE *dataFile = fopen(dataFilename, "r");                      /* Abre arquivo de dados para leitura */
    alloc_check(dataFile, "Falha no processamento do arquivo.\n");

    FILE *BTreeFile = fopen(BTreeFilename, "w+");                   /* Abre arquivo do indice para escrita */
    alloc_check(BTreeFile, "Falha no processamento do arquivo.\n");

    veiculo_header *veiculoHeader = read_binary_veiculo_header(dataFile);   /* Lê o cabeçalho do arquivo veiculo */

    BTree_header *BTreeHeader = calloc(1, sizeof(BTree_header));    /* Inicializa o cabecalho do arquivo de indice  */
    BTreeHeader->status = '0';                                      /*  com staus inconsistente                     */
    BTreeHeader->noRaiz = -1;

    write_btree_header(BTreeHeader, BTreeFile);                      /* Atualiza o arquivo de indice com o cabecalho */

    int totalDeRegistros = veiculoHeader->nroRegistros + veiculoHeader->nroRegRemovidos;    /* Le registro por registro do   */
    for(int i = 0; i < totalDeRegistros; i++)                                               /*  arquivo de dados e insere as */
    {                                                                                       /*  chaves no arquivo de indice  */
        long long int reference = ftell(dataFile);

        veiculo_data *data = read_binary_veiculo_data(dataFile);

        if(data->removido == '1'){                                                      /* Se o registro não estiver removido */
            insert(convertePrefixo(data->prefixo), reference, BTreeHeader, BTreeFile);  /*  insere no indice                  */
        } else {                                                                        /* Caso o contrario, pula para o      */
            fseek(dataFile, data->tamanhoRegistro, SEEK_CUR);                           /*  próximo registro de veiculo       */
        }

        free(data);
    }

    BTreeHeader->status = '1';                  /* Atualiza o status do arquivo     */
    write_btree_header(BTreeHeader, BTreeFile); /* Atualiza o cabecalho do arquivo  */
    
    free(veiculoHeader);
    free(BTreeHeader);
    fclose(dataFile);
    fclose(BTreeFile);
    binarioNaTela(BTreeFilename);
}


void comando_10()
{
    /**
     *  Esse comando permitir criar um arquivo de indice (arvore-B)
     *  para o arquivo de dados linha 
     */

    char dataFilename[128], BTreeFilename[128];         /* Recebe as primeiras duas entradas depois do comando  */
    scanf("%s %s", dataFilename, BTreeFilename);        /* ex entrada: 10 linha.bin indicePrefixo.bin           */

    FILE *dataFile = fopen(dataFilename, "r");                      /* Abre arquivo de dados para leitura */
    alloc_check(dataFile, "Falha no processamento do arquivo.\n");

    FILE *BTreeFile = fopen(BTreeFilename, "w+");                   /* Abre arquivo do indice para escrita */
    alloc_check(BTreeFile, "Falha no processamento do arquivo.\n");

    linha_header *linhaHeader = read_binary_linha_header(dataFile); /* Lê o cabeçalho do arquivo linha */

    BTree_header *BTreeHeader = calloc(1, sizeof(BTree_header));    /* Inicializa o cabecalho do arquivo de indice  */
    BTreeHeader->status = '0';                                      /*  com staus inconsistente                     */
    BTreeHeader->noRaiz = -1;
   
    write_btree_header(BTreeHeader, BTreeFile);                     /* Atualiza o arquivo de indice com o cabecalho */

    int totalDeRegistros = linhaHeader->nroRegistros + linhaHeader->nroRegRemovidos;    /* Le registro por registro do   */
    for(int i = 0; i < totalDeRegistros; i++)                                           /*  arquivo de dados e insere as */
    {                                                                                   /*  chaves no arquivo de indice  */
        long long int reference = ftell(dataFile);
        
        linha_data *data = read_binary_linha_data(dataFile);
        
        if(data->removido == '1')                                           /* Se o registro não estiver removido */
            insert(data->codLinha, reference, BTreeHeader, BTreeFile);      /*  insere no indice                  */
        else                                                                /* Caso o contrario, pula para o      */
            fseek(dataFile, data->tamanhoRegistro, SEEK_CUR);               /*  próximo registro de veiculo       */
        free(data);
    }

    BTreeHeader->status = '1';                      /* Atualiza o status do arquivo     */
    write_btree_header(BTreeHeader, BTreeFile);     /* Atualiza o cabecalho do arquivo  */

    free(linhaHeader);
    free(BTreeHeader);
    fclose(dataFile);
    fclose(BTreeFile);
    binarioNaTela(BTreeFilename);
}


void comando_11()
{
    /**
     * Comando usado para recuperar um registro do tipo veiculo
     *  pelo campo prefixo
     */

    char dataFilename[128], BTreeFilename[128], prefix[128];    /* Recebe as primeiras duas entradas depois do comando          */
    scanf("%s %s prefixo ", dataFilename, BTreeFilename);       /* ex entrada: 11 veiculo.bin indicePrefixo.bin prefixo "GD343" */
    scan_quote_string(prefix);      

    FILE *dataFile = fopen(dataFilename, "r");                      /* Abre arquivo de dados para leitura */
    alloc_check(dataFile, "Falha no processamento do arquivo.\n");

    FILE *BTreeFile = fopen(BTreeFilename, "r");                    /* Abre arquivo do indice para leitura */
    alloc_check(BTreeFile, "Falha no processamento do arquivo.\n");

    veiculo_header *dataHeader = read_binary_veiculo_header(dataFile);  /* Lê o cabeçalho do arquivo veiculo   */

    BTree_header *BTreeHeader = read_btree_header(BTreeFile);           /* Lê o cabeçalho do arquivo de indice */

    int key = convertePrefixo(prefix);                              /* Converte o prefixo para um modo      */
    long long int ref;                                              /*  indexado à inteiros                 */
    int res = search(BTreeHeader->noRaiz, key, BTreeFile, &ref);    /* Busca o prefixo no arquivo de indice */

    if(res == NOT_FOUND)                    /* Caso não tenha encontrado */
        printf("Registro inexistente.\n");
    else{                                   /* Caso tenha encontrado */
        fseek(dataFile, ref, SEEK_SET);     /* Acessa a posição encontrada */

        veiculo_data *data = read_binary_veiculo_data(dataFile);    /* Lê o registro */

        print_veiculo_data(dataHeader, data);   /* Imprime os dados */
        printf("\n");
        free(data);
    }

    fclose(dataFile);
    fclose(BTreeFile);
    free(dataHeader);
    free(BTreeHeader);
}

void comando_12()
{
    /**
     * Comando usado para recuperar um registro do tipo linha
     *  pelo campo prefixo
     */

    int codLinha;
    char dataFilename[128], BTreeFilename[128];                             /* Recebe as primeiras duas entradas depois do comando       */
    scanf("%s %s codLinha %d", dataFilename, BTreeFilename, &codLinha);     /* ex entrada: 12 linha.bin indicePrefixo.bin codLinha 659   */

    FILE *dataFile = fopen(dataFilename, "r");                      /* Abre arquivo de dados para leitura */
    alloc_check(dataFile, "Falha no processamento do arquivo.\n");

    FILE *BTreeFile = fopen(BTreeFilename, "r");                    /* Abre arquivo do indice para leitura */
    alloc_check(BTreeFile, "Falha no processamento do arquivo.\n");

    linha_header *dataHeader = read_binary_linha_header(dataFile);  /* Lê o cabeçalho do arquivo linha */

    BTree_header *BTreeHeader = read_btree_header(BTreeFile);       /* Lê o cabeçalho do arquivo de indice */

    long long int ref;
    int res = search(BTreeHeader->noRaiz, codLinha, BTreeFile, &ref);   /* Busca o prefixo no arquivo de indice */

    if(res == NOT_FOUND)                    /* Caso não tenha encontrado */
        printf("Registro inexistente.\n");
    else{                                   /* Caso tenha encontrado */
        fseek(dataFile, ref, SEEK_SET);     /* Acessa a posição encontrada */

        linha_data *data = read_binary_linha_data(dataFile);    /* Lê o registro */

        print_linha_data(dataHeader, data);     /* Imprime os dados */
        printf("\n");
        free(data);
    }

    fclose(dataFile);
    fclose(BTreeFile);
    free(dataHeader);
    free(BTreeHeader);
}


void comando_13()
{
    /**
     * Extensao do comando 7: Usado para ler entradas da entrada padrao e escrever no arquivo binario de veiculo, 
     * guardando as chaves e referencias em um arquivo de indice arcore-B
    */

    int n;      /* Armazenar o numero de entradas */
    char dataFilename[128], BTreeFilename[128], prefix[128];    /* Recebe as primeiras duas entradas depois do comando */
    scanf("%s %s %d", dataFilename, BTreeFilename, &n);         /* ex entrada: 13 veiculo.bin indicePrefixo.bin 4      */

    FILE *dataFile = fopen(dataFilename, "r+");                     /* Abre arquivo de dados para leitura e escrita */
    alloc_check(dataFile, "Falha no processamento do arquivo.\n");

    FILE *BTreeFile = fopen(BTreeFilename, "r+");                   /* Abre arquivo do indice para leitura e escrita */
    alloc_check(BTreeFile, "Falha no processamento do arquivo.\n");

    veiculo_header *dataHeader = read_binary_veiculo_header(dataFile);  /* Lê o cabeçalho do arquivo veiculo */

    BTree_header *BTreeHeader = read_btree_header(BTreeFile);           /* Lê o cabeçalho do arquivo de indice */

    dataHeader->status = '0';           /* Atribui inconscistencia nos arquivos*/
    BTreeHeader->status = '0';

    update_binary_veiculo_header(dataHeader, dataFile); /* Atualiza o header do arquivo de veiculo */

    write_btree_header(BTreeHeader, BTreeFile);         /* Atualiza o arquivo de indice */

    veiculo_data *data;
    for(int i = 0; i < n; i++){                                 /* Para cada novo registro lido da entrada padrão  */
        long long int reference = dataHeader->byteProxReg;      /*  escreve o registro no arquivo de veiculo e     */
        data = read_stdin_veiculo_data(dataHeader);             /*  insere o indice no arquivo de indices arvore B */

        append_binary_veiculo_data(dataHeader, data, dataFile);

        insert(convertePrefixo(data->prefixo), reference, BTreeHeader, BTreeFile);
        free(data);
    }

    dataHeader->status = '1';                           /* Altera o status do arquivo  */
    BTreeHeader->status = '1';                          /*  e atualiza o header dos    */
    update_binary_veiculo_header(dataHeader, dataFile); /*  arquivo de veiculo e indice*/
    write_btree_header(BTreeHeader, BTreeFile);

    fclose(dataFile);
    fclose(BTreeFile);
    free(dataHeader);
    free(BTreeHeader);
    binarioNaTela(BTreeFilename);
}


void comando_14()
{
    /**
     * Extensao do comando 8: Usado para ler entradas da entrada padrao e escrever no arquivo binario de linha, 
     * guardando as chaves e referencias em um arquivo de indice arcore-B
    */

    int n;      /* Armazenar o numero de entradas */
    char dataFilename[128], BTreeFilename[128], prefix[128];    /* Recebe as primeiras duas entradas depois do comando */
    scanf("%s %s %d", dataFilename, BTreeFilename, &n);         /* ex entrada: 14 linha.bin indicePrefixo.bin 4        */

    FILE *dataFile = fopen(dataFilename, "r+");                     /* Abre arquivo de dados para leitura e escrita */
    alloc_check(dataFile, "Falha no processamento do arquivo.\n");

    FILE *BTreeFile = fopen(BTreeFilename, "r+");                   /* Abre arquivo do indice para leitura e escrita */
    alloc_check(BTreeFile, "Falha no processamento do arquivo.\n");

    linha_header *dataHeader = read_binary_linha_header(dataFile);  /* Lê o cabeçalho do arquivo veiculo */

    BTree_header *BTreeHeader = read_btree_header(BTreeFile);       /* Lê o cabeçalho do arquivo de indice */

    dataHeader->status = '0';               /* Atribui inconscistencia nos arquivos*/
    BTreeHeader->status = '0';

    update_binary_linha_header(dataHeader, dataFile);   /* Atualiza o header do arquivo de linha */

    write_btree_header(BTreeHeader, BTreeFile);          /* Atualiza o arquivo de indice */

    linha_data *data;
    for(int i = 0; i < n; i++){                             /* Para cada novo registro lido da entrada padrão   */
        long long int reference = dataHeader->byteProxReg;  /*  escreve o registro no arquivo de linha e insere */
        data = read_stdin_linha_data(dataHeader);           /*  o indice no arquivo de indices arvore B         */
        
        append_binary_linha_data(dataHeader, data, dataFile);

        insert(data->codLinha, reference, BTreeHeader, BTreeFile);
        free(data);
    }

    dataHeader->status = '1';                           /* Altera o status do arquivo */
    BTreeHeader->status = '1';                          /*  e atualiza o header dos   */
    update_binary_linha_header(dataHeader, dataFile);   /*  arquivo de linha e indice */
    write_btree_header(BTreeHeader, BTreeFile);

    fclose(dataFile);
    fclose(BTreeFile);
    free(dataHeader);
    free(BTreeHeader);
    binarioNaTela(BTreeFilename);
}


void comando_15()
{
    /**
     *   Permite a recuperação dos registros de dados em veiculo.bin, 
     *   juntando-os de forma apropriada com os dados de linha.bin.
     *   Usa loops aninhados
     *   Apenas o campo codLinha pode ser utilizado
     */
    // Recebe as entradas do comando
    char veiculoFilename[128], linhaFilename[128];
    scanf("%s %s codLinha codLinha", veiculoFilename, linhaFilename);

    // Abre os arquivos binarios para leitura
    FILE *veiculoBin = fopen(veiculoFilename, "r");
    alloc_check(veiculoBin, "Falha no processamento do arquivo.\n");
    FILE *linhaBin = fopen(linhaFilename, "r");
    alloc_check(linhaBin, "Falha no processamento do arquivo.\n");

    // Le cabecalho dos arquivos binarios
    veiculo_header *veiculoHeader = read_binary_veiculo_header(veiculoBin);
    linha_header *linhaHeader = read_binary_linha_header(linhaBin);

    int totalVeiculo = veiculoHeader->nroRegistros + veiculoHeader->nroRegRemovidos;
    int totalLinha = linhaHeader->nroRegistros + linhaHeader->nroRegRemovidos;
    int existeRegistro = 0, flag = 0;

    // Para cada registro em 'veiculo'
    for(int i = 0; i < totalVeiculo; i++){
        veiculo_data *veiculoData = read_binary_veiculo_data(veiculoBin);
        // Pula o registro veiculo se estiver removido
        if(veiculoData->removido == '0'){
            fseek(veiculoBin, veiculoData->tamanhoRegistro, SEEK_CUR);
        } 
        else {
            flag = 0;
            // Posiciona o ponteiro do arquivo linha no inicio
            fseek(linhaBin, LINHA_HEADER_SIZE, SEEK_SET);
            // Para cada registro em 'linha'
            for(int j = 0; j < totalLinha && flag == 0; j++){
                linha_data *linhaData = read_binary_linha_data(linhaBin);
                // Pula o registro linha se estiver removido
                if(linhaData->removido == '0'){
                    fseek(linhaBin, linhaData->tamanhoRegistro, SEEK_CUR);
                } 
                else if (veiculoData->codLinha == linhaData->codLinha){
                    print_veiculo_data(veiculoHeader, veiculoData);
                    print_linha_data(linhaHeader, linhaData);
                    printf("\n");
                    flag = 1; existeRegistro = 1;
                }
                free(linhaData);
            }
        }
        free(veiculoData);
    }

    if(existeRegistro == 0)
        printf("Registro inexistente.\n");

    free(veiculoHeader);
    free(linhaHeader);
    fclose(veiculoBin);
    fclose(linhaBin);
}


void comando_16()
{
    /**
     *   Permite a recuperação dos registros de dados em veiculo.bin, 
     *   juntando-os de forma apropriada com os dados de linha.bin.
     *   Usa um loop unico com a busca da arvore B
     *   Apenas o campo codLinha pode ser utilizado
     */
    // Recebe as entradas do comando
    char veiculoFilename[128], linhaFilename[128], indexFilename[128];
    scanf("%s %s codLinha codLinha %s", veiculoFilename, linhaFilename, indexFilename);

    // Abre os arquivos binarios para leitura
    FILE *veiculoBin = fopen(veiculoFilename, "r");
    alloc_check(veiculoBin, "Falha no processamento do arquivo.\n");
    FILE *linhaBin = fopen(linhaFilename, "r");
    alloc_check(linhaBin, "Falha no processamento do arquivo.\n");
    FILE *bTreeFile = fopen(indexFilename, "r");
    alloc_check(bTreeFile, "Falha no processamento do arquivo.\n");

    // Le cabecalho dos arquivos binarios
    veiculo_header *veiculoHeader = read_binary_veiculo_header(veiculoBin);
    linha_header *linhaHeader = read_binary_linha_header(linhaBin);
    BTree_header *bTreeHeader = read_btree_header(bTreeFile);

    int totalVeiculo = veiculoHeader->nroRegistros + veiculoHeader->nroRegRemovidos;
    int existeRegistro = 0, res;
    long long int ref;

    // Para cada registro em 'veiculo'
    for(int i = 0; i < totalVeiculo; i++){
        veiculo_data *veiculoData = read_binary_veiculo_data(veiculoBin);
        // Pula o registro veiculo se estiver removido
        if(veiculoData->removido == '0'){
            fseek(veiculoBin, veiculoData->tamanhoRegistro, SEEK_CUR);
        } 
        else {
            // Faz a busca usando o arquivo de indice
            res = search(bTreeHeader->noRaiz, veiculoData->codLinha, bTreeFile, &ref);
            if(res == FOUND){
                // Le o registro da posicao encontrada
                fseek(linhaBin, ref, SEEK_SET);
                linha_data *linhaData = read_binary_linha_data(linhaBin);
                // Imprime os registros
                print_veiculo_data(veiculoHeader, veiculoData);
                print_linha_data(linhaHeader, linhaData);
                printf("\n");
                existeRegistro = 1;
                free(linhaData);
            }
                
        }
        free(veiculoData);
    }

    if(existeRegistro == 0)
        printf("Registro inexistente.\n");

    free(bTreeHeader);
    free(veiculoHeader);
    free(linhaHeader);
    fclose(veiculoBin);
    fclose(linhaBin);
    fclose(bTreeFile);
}

void comando_17()
{
    /**
     *  Cria um novo arquivo a partir do arquivo de dados veiculo de
     *  maneira ordenada
     */
    // Recebe as entradas do comando
    char unorderedFilename[128], orderedFilename[128];
    scanf("%s %s codLinha", unorderedFilename, orderedFilename);

    // Abre o arquivo nao ordenado para leitura
    FILE *unorderedFile = fopen(unorderedFilename, "r");
    alloc_check(unorderedFile, "Falha no processamento do arquivo.\n");
    // Abre um novo arquivo para escrita
    FILE *orderedFile = fopen(orderedFilename, "w");
    alloc_check(orderedFile, "Falha no processamento do arquivo.\n");

    generate_ordered_veiculo_file(unorderedFile, orderedFile);

    //Libera memoria
    fclose(unorderedFile);
    fclose(orderedFile);
    binarioNaTela(orderedFilename);
}

void comando_18()
{
    /**
     *  Cria um novo arquivo a partir do arquivo de dados linha de
     *  maneira ordenada
     */
    // Recebe as entradas do comando
    char unorderedFilename[128], orderedFilename[128];
    scanf("%s %s codLinha", unorderedFilename, orderedFilename);

    // Abre o arquivo nao ordenado para leitura
    FILE *unorderedFile = fopen(unorderedFilename, "r");
    alloc_check(unorderedFile, "Falha no processamento do arquivo.\n");
    // Abre um novo arquivo para escrita
    FILE *orderedFile = fopen(orderedFilename, "w");
    alloc_check(orderedFile, "Falha no processamento do arquivo.\n");

    generate_ordered_linha_file(unorderedFile, orderedFile);
    
    fclose(unorderedFile);
    fclose(orderedFile);
    binarioNaTela(orderedFilename);
}


void comando_19()
{
    /**
     *   Permite a recuperação dos registros de dados em veiculo.bin, 
     *   juntando-os de forma eficiente com os dados de linha.bin.
     *   Utiliza o campo codLinha de ambos arquivos veiculo.bin e linha.bin
     *      para relacionar os arquivos.
     */
    // Recebe as entradas do comando
    char veiculoFilename[128], linhaFilename[128];
    FILE *unorderedVeiculoFile,     /* Pointer para o arquivo desordenado de veiculo */
         *orderedVeiculoFile,       /* Pointer para o arquivo ordenado de veiculo */
         *unorderedLinhaFile,       /* Pointer para o arquivo desordenado de linha */
         *orderedLinhaFile;         /* Pointer para o arquivo ordenado de linha */
    veiculo_header *veiculoHeader;  /* Header do arquivo veiculo ordenado */
    linha_header *linhaHeader;      /* Header do arquivo linha ordenado*/
    veiculo_data *veiculoData;      /* Data struct para o arquivo veiculo */
    linha_data *linhaData;          /* Data struct para o arquivo linha */
    int veiculoCounter,             /* Contador do registro de veiculo iterado */
        linhaCounter;               /* Contador do registro de linha iterado */
    int registerFound;              /* boolean: Flag pra registro encontrado */

    scanf("%s %s codLinha codLinha", veiculoFilename, linhaFilename);
    
    /* Abrir arquivos */
    unorderedVeiculoFile = fopen(veiculoFilename, "r");
    orderedVeiculoFile = fopen(ORDERED_VEICULO_FILENAME, "w+");

    unorderedLinhaFile = fopen(linhaFilename, "r");
    orderedLinhaFile = fopen(ORDERED_LINHA_FILENAME, "w+");
   
    /* Gerar os arquivos ordenados com as funcoes do comando 17 e 18 */
    generate_ordered_veiculo_file(unorderedVeiculoFile, orderedVeiculoFile);

    generate_ordered_linha_file(unorderedLinhaFile, orderedLinhaFile);
    
    veiculoHeader = read_binary_veiculo_header(orderedVeiculoFile);
    linhaHeader = read_binary_linha_header(orderedLinhaFile);
    
    veiculoCounter = 0;
    veiculoData = read_binary_veiculo_data(orderedVeiculoFile);

    linhaCounter = 0;
    linhaData = read_binary_linha_data(orderedLinhaFile);
    
    registerFound = 0;

    while (veiculoCounter < veiculoHeader->nroRegistros && linhaCounter < linhaHeader->nroRegistros)
    {
        if (veiculoData->codLinha < linhaData->codLinha) {
            veiculoCounter++;
            free(veiculoData);
            veiculoData = read_binary_veiculo_data(orderedVeiculoFile);
        } else if (veiculoData->codLinha > linhaData->codLinha) {
            free(linhaData);
            linhaCounter++;
            linhaData = read_binary_linha_data(orderedLinhaFile);
        } else {
            registerFound = 1;

            print_veiculo_data(veiculoHeader, veiculoData);
            print_linha_data(linhaHeader, linhaData);
            printf("\n");
        
            veiculoCounter++;
            free(veiculoData);
            veiculoData = read_binary_veiculo_data(orderedVeiculoFile);
        }
    }

    if (registerFound == NOT_FOUND) {
        printf("Registro inexistente.\n");
    }
    
    free(veiculoHeader);
    free(veiculoData);
    free(linhaHeader);
    free(linhaData);
    fclose(unorderedVeiculoFile);
    fclose(orderedVeiculoFile);
    fclose(unorderedLinhaFile);
    fclose(orderedLinhaFile);
}
