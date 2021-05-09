# Trabalho-Banco-de-Dados
Trabalho Prático - Disciplina Organização de Arquivos 

---

## Comandos para executar o projeto
1. Compila o código
```
make all
```

2. Compila o código com a tag -g e define DEBUG para utilizacao no código
```
make debug
```

3. Roda o código executavel
```
make run
```

4. Exclui o executavel compilado
```
make clear
```

5. Executa o programa valgrind com o executavel passado como parametro:
```
make memleak
```

---

## Estrutura de pastas do projeto

- **src**: .c com funcoes do código
- **include**: .h do código
- **tests**: Onde estará os casos teste liberados
- **main.h**: Programa principal do código
- **Makefile**: Arquivo utilitário para compilacao e execucao de código
