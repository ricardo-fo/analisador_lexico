#include "queue.h"
#include <stdlib.h> // malloc
#include <string.h> // strcpy

// Cria uma fila
Queue * create_queue() {
    Queue * queue = (Queue *)malloc(sizeof(Queue));
    queue->top = 0;
    queue->end = -1;
    queue->nTokens = 0;
    queue->limit = MAX_TOKENS;

    return queue;
}

// Insere um novo elemento na fila
void enqueue(Queue * queue, char token[]) {
    if(queue->end == queue->limit - 1)
        queue->end = -1;

    queue->end++;
    strcpy(queue->tokens[queue->end], token);
    queue->nTokens++;
}
