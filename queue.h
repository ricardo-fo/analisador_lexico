#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include <stdbool.h> // bool

#define MAX_TOKENS 1000
#define MAX_STR_SIZE 30

typedef struct {
    int top;
    int end;
    int limit;
    int nTokens;
    char tokens[MAX_TOKENS][MAX_STR_SIZE];
} Queue;

/* Cria uma fila. */
Queue * create_queue();

/* Insere um novo elemento na fila. */
void enqueue(Queue *, char *);

/* Remove um elemento da fila. */
char * dequeue(Queue *);

/* Verifica se uma fila está vazia. */
bool is_empty_queue(Queue *);

/* Verifica se uma fila está cheia. */
bool is_full_queue(Queue *);

/* Imprime o conteúdo de uma fila. */
void print_queue(Queue *);

#endif // QUEUE_H_INCLUDED
