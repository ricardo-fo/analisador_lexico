##### Variáveis #####
# Compilador
CC=gcc

# Flags
CFLAGS=-W -Wall

# Executável
EXEC=aplicacao

# Nome do executavel
PROJ_NAME=aplicacao

# Arquivos source, i.e. extensão .c
C_SRC=$(wildcard *.c)

# Arquivos objeto, i.e. extensão .o
OBJ=$(C_SRC:.c=.o)

##### Regras e dependências #####
$(PROJ_NAME): $(OBJ)
	$(CC) -o $(PROJ_NAME) $(OBJ) $(CFLAGS) -lm
	@echo "\nCompilação finalizada!\nDigite 'make run' e tecle <enter> para executar a aplicacao"

main.o: main.c util.o token.o lexical_analyzer.o
	$(CC) -c main.c $(CFLAGS)

util.o: util.h
	$(CC) -c util.c $(CFLAGS)

token.o: token.h
	$(CC) -c token.c $(CFLAGS)

lexical_analyzer.o: lexical_analyzer.h stack.h token.h
	$(CC) -c lexical_analyzer.c $(CFLAGS)

stack.o: stack.h
	$(CC) -c stack.c $(CFLAGS)

# Executar
run:
	./$(EXEC)

# Limpar de arquivos
clean:
	rm -rf *.o

# Apagar tudo o que deve ser modificado
mrproper:
	rm -rf $(EXEC)
