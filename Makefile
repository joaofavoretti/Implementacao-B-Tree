# Trabalho Prático 1 - Organizacao de Arquivos
# Nome: João Pedro Favoretti (11316055)
# Nome:

CC = gcc
MAIN = main.c
PROG = tests/main
FUNCTIONS = src/*.c funcao-fornecida.c
INCLUDE = include

all: main.c
	@$(CC) $(MAIN) $(FUNCTIONS) -I $(INCLUDE) -o $(PROG)

run:
	@./$(PROG)

debug:
	@$(CC) $(MAIN) $(FUNCTIONS) -I $(INCLUDE) -o $(PROG) -D DEBUG -g

clear:
	rm $(PROG)
	
memleak:
	@valgrind --leak-check=full --show-leak-kinds=all ./$(PROG)
	