/**
 * Trabalho Prático 3 - Organizacao de Arquivos
 * Nome: João Pedro Favoretti (11316055)
 * Nome: Lucas Pilla (10633328)
 */

#include <util.h>

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

// Funcao de comparacao usada no qsort
int cmp_veiculo(const void *a, const void *b)
{

    veiculo_data *r = * (veiculo_data **) a;
    veiculo_data *s = * (veiculo_data **) b;

    if(r->codLinha > s->codLinha)
        return 1;
    else if(r->codLinha < s->codLinha)
        return -1;
    else
        return 0;
}

// Funcao de comparacao usada no qsort
int cmp_linha(const void *a, const void *b)
{

    linha_data *r = * (linha_data **) a;
    linha_data *s = * (linha_data **) b;

    if(r->codLinha > s->codLinha)
        return 1;
    else if(r->codLinha < s->codLinha)
        return -1;
    else
        return 0;
}

void generate_ordered_veiculo_file (FILE *unorderedFile, FILE *orderedFile)
{
    // Le cabecalho
    veiculo_header *orderedHeader = read_binary_veiculo_header(unorderedFile);
    orderedHeader->status = '0';
    orderedHeader->byteProxReg = VEICULO_HEADER_SIZE;

    // Escreve o cabecalho no novo arquivo com status 'inconsistente'
    update_binary_veiculo_header(orderedHeader, orderedFile);

    int total = orderedHeader->nroRegistros + orderedHeader->nroRegRemovidos;
    int numRegistros = 0;

    veiculo_data **data = NULL;

    // Carrega todos os registros nao removidos na RAM
    for(int i = 0; i < total; i++){
        veiculo_data *currData = read_binary_veiculo_data(unorderedFile);
        if(currData->removido == '1'){
            data = realloc(data, (numRegistros+1)*sizeof(veiculo_data *));
            data[numRegistros] = currData;
            numRegistros++;
        } else {
            fseek(unorderedFile, currData->tamanhoRegistro, SEEK_CUR);
            free(currData);
        }
    }

    // Ordena os registros
    qsort(data, numRegistros, sizeof(veiculo_data *), cmp_veiculo);

    // Escreve os registros ordenados no novo arquivo
    for(int i = 0; i < numRegistros; i++)
        append_binary_veiculo_data(orderedHeader, data[i], orderedFile);

    // Atualiza valores do header
    orderedHeader->nroRegistros = numRegistros;
    orderedHeader->nroRegRemovidos = 0;
    orderedHeader->status = '1';
    update_binary_veiculo_header(orderedHeader, orderedFile);

    //Libera memoria
    for(int i = 0; i < numRegistros; i++)
        free(data[i]);
    free(data);
    free(orderedHeader);
}

void generate_ordered_linha_file (FILE *unorderedFile, FILE *orderedFile)
{
    // Le cabecalho
    linha_header *orderedHeader = read_binary_linha_header(unorderedFile);
    orderedHeader->status = '0';
    orderedHeader->byteProxReg = LINHA_HEADER_SIZE;

    // Escreve o cabecalho no novo arquivo com status 'inconsistente'
    update_binary_linha_header(orderedHeader, orderedFile);

    int total = orderedHeader->nroRegistros + orderedHeader->nroRegRemovidos;
    int numRegistros = 0;
    linha_data **data = NULL;

    // Carrega todos os registros nao removidos na RAM
    for(int i = 0; i < total; i++){
        linha_data *currData = read_binary_linha_data(unorderedFile);
        if(currData->removido == '1'){
            data = realloc(data, (numRegistros+1)*sizeof(linha_data *));
            data[numRegistros] = currData;
            numRegistros++;
        } else {
            fseek(unorderedFile, currData->tamanhoRegistro, SEEK_CUR);
            free(currData);
        }
    }

    // Ordena os registros
    qsort(data, numRegistros, sizeof(linha_data *), cmp_linha);

    // Escreve os registros ordenados no novo arquivo
    for(int i = 0; i < numRegistros; i++)
        append_binary_linha_data(orderedHeader, data[i], orderedFile);

    // Atualiza valores do header
    orderedHeader->nroRegistros = numRegistros;
    orderedHeader->nroRegRemovidos = 0;
    orderedHeader->status = '1';
    update_binary_linha_header(orderedHeader, orderedFile);

    //Libera memoria
    for(int i = 0; i < numRegistros; i++)
        free(data[i]);

    free(data);
    free(orderedHeader);
}

void binarioNaTela(char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}


void scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}


int convertePrefixo(char* str) {

    /* O registro que tem essa string como chave foi removido */
    if(str[0] == '*')
        return -1;

    /* Começamos com o primeiro digito na ordem de 36^0 = 1 */
    int power = 1;

    /* Faz a conversão char por char para chegar ao resultado */
    int result = 0;
    for(int i = 0; i < 5; i++) {

        /* 
            Interpreta o char atual como se fosse um digito
            em base 36. Os digitos da base 36 são:
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D,
            E, F, G, H, I, J, K, L, M, N, O, P, Q, R,
            S, T, U, V, W, X, Y, Z
        */
        int cur_digit;
        /* Checa pelos digitos normais e os converte para números */
        if(str[i] >= '0' && str[i] <= '9')
            cur_digit = str[i] - '0';
        /* Checa pelas letras e as converte para números */
        else if(str[i] >= 'A' && str[i] <= 'Z')
            cur_digit = 10 + str[i] - 'A';

        /*
            Multiplica o digito atual pelo ordem da posição atual
            e adiciona no resultado
            Primeira posição:   36^0 = 1
            Segunda posição:    36^1 = 36
            Terceira posição:   36^2 = 1.296
            Quarta posição:     36^3 = 46.656
            Quinta posição:     36^4 = 1.679.616
        */
        result += cur_digit * power;

        /* Aumenta a ordem atual */
        power *= 36;

    }

    return result;

}


/* ---------------- EXTRA ----------------

OPCIONAL: dicas sobre scanf() e fscanf():

scanf("%[^,]", string) -> lê até encontrar o caractere ',', não incluindo o mesmo na leitura.

Exemplo de entrada: "Oi, esse é um exemplo."
Nesse caso, o scanf("%[^,]") tem como resultado a string "Oi";

scanf("%[^\"]", string) -> lê até encontrar o caractere '"', não incluindo o mesmo na leitura.
scanf("%[^\n]", string) -> lê até encontrar o fim da linha, não incluindo o '\n' na leitura.

scanf("%*c") --> lê um char e não guarda em nenhuma variável, como se tivesse ignorado ele

*/
