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

char * readString(); /* Lê uma string do teclado. */

/*
 * Struct: stack
 * -------------
 * Estrutura de uma pilha.
 */

struct Stack {
    int begin;
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

/*
 * Struct: Queue
 * -------------
 * Estrutura de uma lista.
 */

struct Queue {
    int top;
    int end;
    int limit;
    int nTokens;
    char tokens[MAX_SIZE][MAX_SIZE];
};

struct Queue * createQueue();         /* Cria uma fila. */
void enqueue(struct Queue *, char *); /* Insere um novo elemento na fila. */
char * dequeue(struct Queue *);       /* Remove um elemento da fila. */
bool isEmptyQueue(struct Queue *);    /* Verifica se uma fila está vazia. */
bool isFullQueue(struct Queue *);     /* Verifica se uma fila está cheia. */
void printQueue(struct Queue *);      /* Imprime o conteúdo de uma fila. */

bool analisadorLexico(char *);
int pegaToken(char [], char[]);
void putToken(char [], char []);
bool analisaTokens(char *);
bool verificaTokenValido(const char []);
bool isOperator(const char []);
bool isNumber(const char []);
bool isCommand(const char []);

int main()
{

    printf("Exemplo de expressao: <4><.><2><*><7><+><log><8>\n");
    printf("Insira sua expressao: ");

    char * expression;
    expression = readString();

    printf("\n");

    if(!analisaTokens(expression)) {
        printf("\nEntrada invalida!\n");
        free(expression);

        return 1;
    }

    if(!analisadorLexico(expression)) {
        printf("\nEntrada invalida!\n");
        free(expression);

        return 1;
    }

    printf("\nEntrada valida!\n");
    free(expression);

    return 0;
}

/*
 * Function: read_string
 * ---------------------
 * Lê uma sequência de caractéres do teclado e cria uma string.
 *
 * returns: um ponteiro para a string gerada.
 */
char * readString() {
    char * str = NULL, ch;
    size_t size = 0;
    int i = 0;

    // Lê os caractéres enquanto não encontrar um ENTER
    while((ch = getchar()) != '\n') {
        if(isspace(ch)) continue;
        str = realloc(str, size);
        size += sizeof(char);
        str[i++] = tolower(ch);
    }

    str[i] = '\0';

    return str;
}

// Varre a string, quebrando-a em tokens e inserindo-os numa fila
bool analisaTokens(char * expression) {
    char aux[(int)(strlen(expression)/sizeof(char))];
    char token[(int)(strlen(expression)/sizeof(char))];
    struct Queue * tokens = createQueue();
    strcpy(aux, expression);

    do {
        if (pegaToken(aux, token) == 1){
            printf("Insira tokens validos!\n");
            return false;
        }

        if (verificaTokenValido(token)) {
            enqueue(tokens, token);
        } else {
            printf("O token '%s' e invalido!\n", token);
            return false;
        }
    } while(( (int)(strlen(aux)/sizeof(char)) ) > 0);
    return true;
}

// Varre a string, quebrando-a em tokens e descrevendo o que são
bool analisadorLexico(char * expression) {
    struct Stack * stack = createStack();

    char aux[(int)(strlen(expression)/sizeof(char))];
    char token[(int)(strlen(expression)/sizeof(char))];
    strcpy(aux, expression);

    bool ultimoCaractereEOperadorBinario = false;
    bool ultimoCaractereENumero = false;
    bool primeiroCaractere = true;
    bool segundoNumero = false;

    bool expressaoValida = true;

    do {

        if (pegaToken(aux, token) == 1) return expressaoValida;

        // caso o token for um numero
        if(strstr("0123456789", token)) {

            char auxInteiro[(int)((strlen(token))/sizeof(char))];

            memset(auxInteiro, 0, strlen(auxInteiro));

            strcat(auxInteiro, token);

            if (pegaToken(aux,  token) == 1) {
                printf("%s => numero inteiro\n", auxInteiro);
                return expressaoValida;
            }

            while(strstr("0123456789", token)) {
                push(stack, token);

                if (pegaToken(aux,  token) == 1) {
                    printf("%s => numero inteiro\n", auxInteiro);
                    printStack(stack);
                    return expressaoValida;
                }
            }

            // vai verificar se o conjunto de tokens pego é um numero inteiro ou com ponto flutuante
            if(strstr(".", token)) {
                printf("%s\n", auxInteiro);
            } else {
                printf("%s => numero inteiro\n", auxInteiro);
            }

            printStack(stack);
            putToken(aux, token);
            memset(token, 0, strlen(token));
            strcat(token, auxInteiro);

        // vai verificar se é um comando
        } else if(strstr(token, "enter")) {
            printf("%s => comando\n", token);
            segundoNumero = !segundoNumero;

        // vai verificar se é um numero com sinal
        } else if(primeiroCaractere && strstr("-", token)) {

            bool valido = false;

            if (pegaToken(aux,  token) == 1) {
                printf("- => erro\n");
                return false;
            }

            while(strstr("0123456789", token)) {
                push(stack, token);
                valido = true;

                if (pegaToken(aux,  token) == 1) {
                    printf("- => sinal negativo\n");
                    printStack(stack);
                    return expressaoValida;
                }
            }

            if(valido) {
                printf("- => sinal negativo\n");
                printStack(stack);

                putToken(aux, token);

                memset(token, 0, strlen(token));
                strcat(token, "-");
            } else {
                expressaoValida = false;

                printf("- => erro\n");
                printStack(stack);

                if (pegaToken(aux, token) == 1) return 1;

                while(!strstr("enter", token)) {
                    printf("%s\n", token);

                    if (pegaToken(aux, token) == 1) return 1;
                }

                printf("%s\n", token);
            }

        // vai verificar se é um numero com ponto flutuante
        } else if(strstr(".", token)) {

            if (pegaToken(aux, token) == 1) {
                printf("%s => erro\n", token);
                return false;
            }

            if(!ultimoCaractereENumero && !primeiroCaractere) {
                expressaoValida = false;

                printf(". => erro\n");

                while(!strstr("enter", token)) {
                    printf("%s\n", token);

                    if (pegaToken(aux, token) == 1) return expressaoValida;
                }

                printf("%s\n", token);
            } else {

                bool valido = false;

                while(strstr("0123456789", token)) {
                    push(stack, token);
                    valido = true;

                    if (pegaToken(aux,  token) == 1) {
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

                        if (pegaToken(aux, token) == 1) return expressaoValida;
                    }

                    printf("%s\n", token);
                }
            }
        // vai verificar se é uma potencia
        } else if(strstr(token, "^") || strstr(token, "E") || strstr(token, "e")) {

            if (pegaToken(aux, token) == 1) {
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

                    if (pegaToken(aux, token) == 1) return expressaoValida;
                }

                printf("%s\n", token);
            } else {

                bool valido = false;

                while(strstr("0123456789", token)) {
                    push(stack, token);

                    valido = true;

                    if (pegaToken(aux, token) == 1) {
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

                        if (pegaToken(aux, token) == 1) return expressaoValida;
                    }

                    printf("%s\n", token);
                }

            }

        // vai verificar se é um operador binario
        } else if(strstr("+-*/", token)) {

            if(segundoNumero && ultimoCaractereENumero) {
                printf("%s => operador binario\n", token);
            } else {
                expressaoValida = false;

                printf("%s => erro\n", token);

                if (pegaToken(aux, token) == 1) return expressaoValida;

                while(!strstr("enter", token)) {
                    printf("%s\n", token);

                    if (pegaToken(aux, token) == 1) return expressaoValida;
                }

                printf("%s\n", token);
            }

        // vai verificar se é um operador unario
        } else if(strstr(token, "log") || strstr(token, "cos") || strstr(token, "sen")) {

            if(ultimoCaractereENumero || ultimoCaractereEOperadorBinario) {
                printf("%s => operador unario\n", token);
            } else {
                printf("%s => erro\n", token);

                if (pegaToken(aux, token) == 1) return expressaoValida;

                while(!strstr("enter", token)) {
                    printf("%s\n", token);

                    if (pegaToken(aux, token) == 1) return expressaoValida;
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

// Busca pelo primeiro token da string
int pegaToken(char expression[], char token[]) {
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

// coloca um token na string
void putToken(char expression[], char token[]) {
    char aux[(int)((strlen(expression)+strlen(token)+2)/sizeof(char))];

    memset(aux, 0, strlen(aux));

    strcat(aux, "<");
    strncat(aux, token, (int)(strlen(token)/sizeof(char)));
    strcat(aux, ">");

    strncat(aux, expression, (int)(strlen(expression)/sizeof(char)));

    memset(expression, 0, strlen(expression));

    strcpy(expression, aux);
}

// Verifica se o token é válido
bool verificaTokenValido(const char token[]) {
    if(isNumber(token)) return true;
    if(isOperator(token)) return true;
    if(isCommand(token)) return true;

    return false;
}

// Verifica se o token é um número ou um dígito que compõe um número
bool isNumber(const char token[]) {
    if( ((int)(strlen(token) / sizeof(char))) > 3) return false;

    char digit = (char)token[1];
    return !(strchr("0123456789E.", digit) == NULL);
}

// Verifica se o token é um operador
bool isOperator(const char token[]) {
    if( ((int)(strlen(token) / sizeof(char))) > 5) return false;

    char operators[8][4] = {"+", "-", "*", "/", "^", "log", "sen", "cos"};
    int i;

    for(i = 0; i < 8; i++){
        if(strlen(token) - 2 != strlen(operators[i])) continue;
        if(strncasecmp(token + 1, operators[i], strlen(operators[i])) == 0) return true;
    }

    return false;
}

// Verificas se o token é um comando
bool isCommand(const char token[]) {
    if( ((int)(strlen(token) / sizeof(char))) > 7) return false;

    char commands[1][6] = {"enter"};
    int i;

    for(i = 0; i < 1; i++){
        if(strlen(token) - 2 != strlen(commands[i])) continue;
        if(strncasecmp(token + 1, commands[i], strlen(commands[i])) == 0) return true;
    }

    return false;
}

// Cria uma fila
struct Queue * createQueue() {
    struct Queue * queue = (struct Queue *)malloc(sizeof(struct Queue));
    queue->top = 0;
    queue->end = -1;
    queue->nTokens = 0;
    queue->limit = MAX_SIZE;

    return queue;
}

// Insere um novo elemento na fila
void enqueue(struct Queue * queue, char token[]) {
    if(queue->end == queue->limit - 1)
        queue->end = -1;

    queue->end++;
    strcpy(queue->tokens[queue->end], token);
    queue->nTokens++;
}

// Cria uma pilha
struct Stack * createStack() {
    struct Stack * stack = (struct Stack *)malloc(sizeof(struct Stack));
    stack->top = -1;
    stack->limit = MAX_SIZE;

    return stack;
}

// Verifica se a pilha está vazia
bool isEmptyStack(struct Stack * stack) {
    return stack->top == -1;
}

// Verifica se a pilha está cheia
bool isFullStack(struct Stack* stack) {
    return stack->top == stack->limit - 1;
}

// Insere um novo elemento na pilha
void push(struct Stack * stack, char * token) {
    if(!isFullStack(stack)) {
        strcpy(stack->tokens[++stack->top], token);
    }
}

// Remove um elemento da pilha
char * pop(struct Stack * stack) {
    if(isEmptyStack(stack)) {
        printf("Pilha vazia.\n");
        return "";
    }

    return stack->tokens[stack->top--];
}

void printStack(struct Stack * stack) {
    while(!isEmptyStack(stack)) {
        printf("%s\n", pop(stack));
    }
}
