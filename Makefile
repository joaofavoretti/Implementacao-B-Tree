# Trabalho Prático 2 - Organizacao de Arquivos
# Nome: João Pedro Favoretti (11316055)
# Nome: Lucas Pilla (10633328)

CC = gcc
MAIN = main.c
PROG = main
FUNCTIONS = src/*.c
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
	