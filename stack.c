#include "stack.h"
#include <stdio.h>  // printf, sizeof
#include <string.h> // strcpy
#include <stdlib.h> // malloc

// Cria uma pilha
Stack * create_stack() {
    Stack * stack = (Stack *)malloc(sizeof(Stack));
    stack->top = -1;
    stack->limit = MAX_TOKENS;

    return stack;
}

// Verifica se a pilha está vazia
bool is_empty_stack(Stack * stack) {
    return stack->top == -1;
}

// Verifica se a pilha está cheia
bool is_full_stack(Stack * stack) {
    return stack->top == stack->limit - 1;
}

// Insere um novo elemento na pilha
void push(Stack * stack, char * token) {
    if(!is_full_stack(stack)) {
        strcpy(stack->tokens[++stack->top], token);
    }
}

// Remove um elemento da pilha
char * pop(Stack * stack) {
    if(is_empty_stack(stack)) {
        printf("Pilha vazia.\n");
        return "";
    }

    return stack->tokens[stack->top--];
}

void print_stack(Stack * stack) {
    while(!is_empty_stack(stack)) {
        printf("desimpilha: %s\n", pop(stack));
    }
}