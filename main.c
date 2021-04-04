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
#include <stdbool.h>
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
bool analisadorLexico(struct Stack *, char *);
int getToken(char [], char[]);
void putToken(char expression[], char token[]);

int main()
{
    char * expression;

    expression = read_string();
    printf("\n");

    struct Stack * stack = createStack();

    bool valido = analisadorLexico(stack, expression);

    if(!valido) {
        printf("\nEntrada invalida!\n");
        free(expression);

        return 1;
    }

    printf("\nEntrada valida!\n");
    free(expression);

    return 0;
}

bool analisadorLexico(struct Stack * stack, char * expression) {
    char aux[(int)(strlen(expression)/sizeof(char))];
    char token[(int)(strlen(expression)/sizeof(char))];
    strcpy(aux, expression);

    bool ultimoCaractereEOperadorBinario = false;
    bool ultimoCaractereENumero = false;
    bool primeiroCaractere = true;
    bool segundoNumero = false;

    bool expressaoValida = true;

    do {

        if (getToken(aux, token) == 1) return expressaoValida;


        if(strstr("0123456789", token)) {
            printf("%s\n", token);

        } else if(strstr(token, "enter")) {
            printf("%s\n", token);
            segundoNumero = !segundoNumero;

        } else if(primeiroCaractere && strstr("-", token)) {

            bool valido = false;

            while(strstr("0123456789", token)) {
                push(stack, token);
                valido = true;

                if (getToken(aux,  token) == 1) {
                    printf("- => sinal\n");
                    printStack(stack);
                    return expressaoValida;
                }
            }

            if(valido) {
                printf("- => sinal\n");
                printStack(stack);

                putToken(aux, token);
            } else {
                expressaoValida = false;

                printf("- => erro\n");
                printStack(stack);

                if (getToken(aux, token) == 1) return 1;

                while(!strstr("enter", token)) {
                    printf("%s\n", token);

                    if (getToken(aux, token) == 1) return 1;
                }

                printf("%s\n", token);
            }

        } else if(strstr(".", token)) {

            if (getToken(aux, token) == 1) {
                printf("%s => erro\n", token);
                return false;
            }

            if(!ultimoCaractereENumero && !primeiroCaractere) {
                expressaoValida = false;

                printf(". => erro\n");

                while(!strstr("enter", token)) {
                    printf("%s\n", token);

                    if (getToken(aux, token) == 1) return expressaoValida;
                }

                printf("%s\n", token);
            } else {

                bool valido = false;

                while(strstr("0123456789", token)) {
                    push(stack, token);
                    valido = true;

                    if (getToken(aux,  token) == 1) {
                        printf(". => ponto fluatuante\n");
                        printStack(stack);
                        return expressaoValida;
                    }
                }

                if(valido) {
                    printf(". => ponto fluatuante\n");
                    printStack(stack);

                    putToken(aux, token);
                } else {
                    expressaoValida = false;

                    printf(". => erro\n");
                    printStack(stack);

                    while(!strstr("enter", token)) {
                        printf("%s\n", token);

                        if (getToken(aux, token) == 1) return expressaoValida;
                    }

                    printf("%s\n", token);
                }
            }

        } else if(strstr(token, "^") || strstr(token, "E") || strstr(token, "e")) {

            if (getToken(aux, token) == 1) {
                printf("%s => erro\n", token);
                return false;
            }

            char auxPotencia[(int)((strlen(token))/sizeof(char))];

            memset(auxPotencia, 0, strlen(auxPotencia));

            strcat(auxPotencia, token);

            if(!ultimoCaractereENumero && !primeiroCaractere) {
                expressaoValida = false;

                printf(". => erro\n");

                while(!strstr("enter", token)) {
                    printf("%s\n", token);

                    if (getToken(aux, token) == 1) return expressaoValida;
                }

                printf("%s\n", token);
            } else {

                bool valido = false;

                while(strstr("0123456789", token)) {
                    push(stack, token);

                    valido = true;

                    if (getToken(aux, token) == 1) {
                        printf("%s => potencia\n", auxPotencia);
                        printStack(stack);
                        return expressaoValida;
                    }
                }

                if(valido) {
                    printf("%s => potencia\n", auxPotencia);

                    printStack(stack);

                    putToken(aux, token);
                } else {
                    printf("%s => erro\n", auxPotencia);

                    printStack(stack);

                    while(!strstr("enter", token)) {
                        printf("%s\n", token);

                        if (getToken(aux, token) == 1) return expressaoValida;
                    }

                    printf("%s\n", token);
                }

            }

        } else if(strstr("+-*/", token)) {

            if(segundoNumero && ultimoCaractereENumero) {
                printf("%s => operador binario\n", token);
            } else {
                expressaoValida = false;

                printf("%s => erro\n", token);

                if (getToken(aux, token) == 1) return expressaoValida;

                while(!strstr("enter", token)) {
                    printf("%s\n", token);

                    if (getToken(aux, token) == 1) return expressaoValida;
                }

                printf("%s\n", token);
            }

        } else if(strstr(token, "log") || strstr(token, "cos") || strstr(token, "sen")) {

            if(ultimoCaractereENumero || ultimoCaractereEOperadorBinario) {
                printf("%s => operador unario\n", token);
            } else {
                printf("%s => erro\n", token);

                if (getToken(aux, token) == 1) return expressaoValida;

                while(!strstr("enter", token)) {
                    printf("%s\n", token);

                    if (getToken(aux, token) == 1) return expressaoValida;
                }

                printf("%s\n", token);
            }
        }

        ultimoCaractereENumero = strstr("0123456789.", token);

        ultimoCaractereEOperadorBinario = strstr("+-*/", token);

        primeiroCaractere = strstr("enter", token);
    } while((int)(strlen(aux)/sizeof(char)) > 0);

    return expressaoValida;
}

int getToken(char expression[], char token[]) {
    char * startAtPtr = strstr(expression, "<"); // Ponteiro para o primeiro '<'
    char * endAtPtr = strstr(expression, ">");   // Ponteiro para o primeiro '>'
    int startAt = (startAtPtr == NULL ? -1 : startAtPtr - expression); // Índice para o primeiro '<'
    int endAt = (endAtPtr == NULL ? -1 : endAtPtr - expression);       // Índice para o primeiro '>'

    if (startAt != 0) return 1;
    if (endAt == -1) return 1;
    if (startAt > endAt) return 1;

    memset(token, 0, strlen(token));

    strncpy(token, expression+startAt+1, ++endAt-2);

    if ((int)(strlen(expression) / sizeof(char) - endAt) == 0) {
        strcpy(expression, "");
    } else {
        char aux[(int)(strlen(expression)/sizeof(char)) - endAt];
        strncpy(aux, expression + endAt, (int)(strlen(expression)/sizeof(char)));
        strcpy(expression, aux);
    }

    return 0;
}

void putToken(char expression[], char token[]) {
    char aux[(int)((strlen(expression)+strlen(token)+2)/sizeof(char))];

    memset(aux, 0, strlen(aux));

    strncat(aux, "<", (int)(strlen("<")/sizeof(char)));
    strncat(aux, token, (int)(strlen(token)/sizeof(char)));
    strncat(aux, ">", (int)(strlen(">")/sizeof(char)));

    strncat(aux, expression, (int)(strlen(expression)/sizeof(char)));

    memset(expression, 0, strlen(expression));

    strcpy(expression, aux);
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
    if(!isFull(stack)) {
        strcpy(stack->tokens[++stack->top], token);
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
        printf("desimpilha: %s\n", pop(stack));
    }
}
