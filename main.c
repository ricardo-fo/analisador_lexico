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
#include <strings.h>
#include <stdbool.h>
#define MAX_SIZE 1000

char * readString(); /* Lê uma string do teclado. */

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
bool isEmptyStack(struct Stack *); /* Verifica se uma pilha está vazia. */
bool isFullStack(struct Stack *);  /* Verifica se uma pilha está cheia. */
void printStack(struct Stack *);   /* Imprime o conteúdo de uma pilha. */

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

bool analisadorLexico(struct Stack *, char *);
int getToken(char [], char[]);
void putToken(char [], char []);
bool checkTokens(struct Queue *, char *);
bool isValidToken(const char []);
bool isOperator(const char []);
bool isNumber(const char []);
bool isCommand(const char []);

int main()
{
    char * expression;
    printf("Exemplo de expressão: <4><.><2><*><7><+><log><8>\n");
    printf("Insira sua expressão: ");
    expression = readString();
    printf("\n");

    struct Queue * tokens = createQueue();
    if(!checkTokens(tokens, expression)) return 1;

    printf("Entrada valida!\n");
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
bool checkTokens(struct Queue * tokens, char * expression) {
    char aux[(int)(strlen(expression)/sizeof(char))];
    char token[(int)(strlen(expression)/sizeof(char))];
    strcpy(aux, expression);

    do {
        if (getToken(aux, token) == 1){
            printf("Insira tokens válidos!\n");
            return false;
        }

        if (isValidToken(token)) {
            enqueue(tokens, token);
        } else {
            printf("O token '%s' é inválido!\n", token);
            return false;
        }
    } while(( (int)(strlen(aux)/sizeof(char)) ) > 0);
    return true;
}

// Busca pelo primeiro token da string
int getToken(char expression[], char token[]) {
    char * startAtPtr = strstr(expression, "<"); // Ponteiro para o primeiro '<'
    char * endAtPtr = strstr(expression, ">");   // Ponteiro para o primeiro '>'
    int startAt = (startAtPtr == NULL ? -1 : startAtPtr - expression); // Índice para o primeiro '<'
    int endAt = (endAtPtr == NULL ? -1 : endAtPtr - expression);       // Índice para o primeiro '>'

    if (startAt != 0) return 1;
    if (endAt == -1 || startAt == -1) return 1;
    if (startAt > endAt) return 1;

    memset(token, 0, strlen(token));     // Limpa o conteúdo do token
    strncpy(token, expression, ++endAt); // Copia o token, sem incluir os '<' e '>'
    token[endAt] = '\0';

    // Verifica se sobraram caractéres na expressão
    if ((int)(strlen(expression) / sizeof(char) - endAt) == 0) {
        strcpy(expression, "");
    } else {
        // Remove o token da string
        char aux[(int)(strlen(expression)/sizeof(char)) - endAt];
        strncpy(aux, expression + endAt, (int)(strlen(expression)/sizeof(char)));
        strcpy(expression, aux);
    }

    return 0;
}

// Verifica se o token é válido
bool isValidToken(const char token[]) {
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
        printf("desimpilha: %s\n", pop(stack));
    }
}
