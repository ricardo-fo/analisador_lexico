/*
 * Alunos:
 * Alexandre Saura.
 * Fábio Thomaz.
 * Ricardo Oliveira.
 * Rodrigo Suarez.
 *
 * Trabalho de Compiladores I - UNISANTOS - março de 2021.
 * Prof.: José Fontebasso Neto.
 */

#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 1000

char * read_string(); /* Lê uma string do teclado. */

/*
 * Struct: stack
 * -------------
 * Estrutura de uma pilha.
 */
struct Stack {
    int top;
    int limit;
    char * tokens[MAX_SIZE];
};

struct Stack * createStack();      /* Cria uma nova pilha. */
void push(struct Stack *, char *); /* Insere um novo elemento na pilha. */
char * pop(struct Stack *);        /* Remove um elemento da pilha. */
int isEmpty();                     /* Verifica se uma pilha está vazia. */
int isFull(struct Stack *);        /* Verifica se uma pilha está cheia. */

int main()
{
    char * str;

    str = read_string();
    printf("input: %s\n", str);

    struct Stack * stack = createStack();
    push(stack, str);
    printf("pilha: %s\n", pop(stack));

    free(str);

    return 0;
}

/*
 * Function: read_string
 * ---------------------
 * Lê uma sequência de caractéres do teclado e cria uma string.
 *
 * returns: um ponteiro para a string gerada.
 */
char * read_string() {
    char * str = NULL, ch;
    size_t size = 0;
    int i = 0;

    // Lê os caractéres enquanto não encontrar um ENTER
    while((ch = getchar()) != '\n') {
        str = realloc(str, size);
        size += sizeof(char);
        str[i++] = ch;
    }

    return str;
}

// Cria uma pilha
struct Stack * createStack() {
    struct Stack * stack = (struct Stack *)malloc(sizeof(struct Stack));
    stack->top = -1;
    stack->limit = MAX_SIZE;

    return stack;
}

// Verifica se a pilha está vazia
int isEmpty(struct Stack * stack) {
    return stack->top == -1;
}

// Verifica se a pilha está cheia
int isFull(struct Stack* stack) {
    return stack->top == stack->limit - 1;
}

// Insere um novo elemento na pilha
void push(struct Stack * stack, char * token) {
    if(isFull(stack)) return;
    stack->tokens[++stack->top] = token;
}

// Remove um elemento da pilha
char * pop(struct Stack * stack) {
    if(isEmpty(stack)) return;

    return stack->tokens[stack->top--];
}
