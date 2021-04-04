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
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 1000

char * read_string(); /* Lê uma string do teclado. */
// char * rtrim(char *);
// char * ltrim(char *);
// char * trim(char *);

/*
 * Struct: stack
 * -------------
 * Estrutura de uma pilha.
 */
struct Stack {
    int top;
    int limit;
    char tokens[MAX_SIZE][MAX_SIZE];
};

struct Stack * createStack();      /* Cria uma nova pilha. */
void push(struct Stack *, char *); /* Insere um novo elemento na pilha. */
char * pop(struct Stack *);        /* Remove um elemento da pilha. */
int isEmpty();                     /* Verifica se uma pilha está vazia. */
int isFull(struct Stack *);        /* Verifica se uma pilha está cheia. */
void printStack(struct Stack *);
int fillStack(struct Stack *, char *);
int getToken(char [], char[]);

int main()
{
    char * expression;

    expression = read_string();
    printf("input: %s\n", expression);

    struct Stack * stack = createStack();
    if(fillStack(stack, expression) == 1) {
        printf("Entrada inválida!\n");
        free(expression);

        return 1;
    }

    printStack(stack);
    free(expression);
    return 0;
}

int fillStack(struct Stack * stack, char * expression) {
    char aux[(int)(strlen(expression)/sizeof(char))];
    char token[(int)(strlen(expression)/sizeof(char))];
    strcpy(aux, expression);

    do {
        if (getToken(aux, token) == 1) return 1;
        printf("<<< token: %s\n", token);
        printf("<<< aux: %s\n", aux);
        printf("<<< lenght: %d\n\n", strlen(aux));
        push(stack, token);
    } while((int)(strlen(aux)/sizeof(char)) > 0);

    return 0;
}

int getToken(char expression[], char token[]) {
    char * startAtPtr = strstr(expression, "<"); // Ponteiro para o primeiro '<'
    char * endAtPtr = strstr(expression, ">");   // Ponteiro para o primeiro '>'
    int startAt = (startAtPtr == NULL ? -1 : startAtPtr - expression); // Índice para o primeiro '<'
    int endAt = (endAtPtr == NULL ? -1 : endAtPtr - expression);       // Índice para o primeiro '>'
    printf("startAt >>> %d\n", startAt);
    printf("endAt >>> %d\n", endAt);

    if (startAt != 0) return 1;
    if (endAt == -1) return 1;
    if (startAt > endAt) return 1;

    strncpy(token, expression, ++endAt);
    printf(">>> token: %s\n", token);

    printf("hmmm: %d\n", (int)(strlen(expression) / sizeof(char)) - endAt);
    if ((int)(strlen(expression) / sizeof(char) - endAt) == 0) {
        printf("entrei no 1\n");
        strcpy(expression, "");
    } else {
        printf("entrei no 2\n");
        char aux[(int)(strlen(expression)/sizeof(char)) - endAt];
        strncpy(aux, expression + endAt, (int)(strlen(expression)/sizeof(char)));
        printf("estoy aqui: %s\n", aux);
        strcpy(expression, aux);
    }

    printf(">>> exp: %s\n", expression);

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
        // if (ch == ' ') continue;
        str = realloc(str, size);
        size += sizeof(char);
        str[i++] = tolower(ch);
    }

    str[i] = '\0';

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
    if(isFull(stack)) {
        printf("Pilha cheia.\n");
    } else {
        strcpy(stack->tokens[++stack->top], token);
        printf("aqui vou eu: %s\n", stack->tokens[stack->top]);
    }
}

// Remove um elemento da pilha
char * pop(struct Stack * stack) {
    if(isEmpty(stack)) {
        printf("Pilha vazia.\n");
        return "";
    }

    return stack->tokens[stack->top--];
}

void printStack(struct Stack * stack) {
    while(!isEmpty(stack)) {
        printf("hehe: %s\n", pop(stack));
    }
}
