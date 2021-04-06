#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdbool.h>

#define MAX_TOKENS 1000
#define MAX_STR_SIZE 30

typedef struct {
    int top;
    int limit;
    char tokens[MAX_TOKENS][MAX_STR_SIZE];
} Stack;

/* Cria uma nova pilha. */
Stack * create_stack();

/* Insere um novo elemento na pilha. */
void push(Stack *, char *);

/* Remove um elemento da pilha. */
char * pop(Stack *);

/* Verifica se uma pilha está vazia. */
bool is_empty_stack(Stack *);

/* Verifica se uma pilha está cheia. */
bool is_full_stack(Stack *);

/* Imprime o conteúdo de uma pilha. */
void print_stack(Stack *);

#endif // STACK_H_INCLUDED