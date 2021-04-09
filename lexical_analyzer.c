#include "lexical_analyzer.h"
#include "stack.h"
#include "token.h"
#include <string.h>
#include <stdbool.h>

void analyse(Queue * tokens) {
    Stack * stack = create_stack();
    char current_token[8];

    // analisar a expressão
    while (!is_empty_queue(tokens)) {
        strcpy(current_token, dequeue(tokens));

        if (is_token_number(current_token, tokens, stack)) continue;
        else if (is_token_operator(current_token, stack)) continue;
        else if (is_token_command(current_token, stack)) continue;

        printf("HMMM\n\n");
    }
}

bool is_token_number(const char token[8], Queue * tokens, Stack * stack) {
    int counter = 0;
    char digit = (char)token[1];

    /**
     * Regras:
     * - Pode ter no máximo 8 dígitos;
     * - Pode iniciar com um número ou um sinal de + ou - ;
     * -
     */
    if (strchr("0123456789", digit) != NULL || strchr("-+", digit) != NULL) {
    }

    return false;
}

// Verifica se o token é um operador e insere ele na pilha
bool is_token_operator(const char token[8], Stack * stack) {
    if (is_operator(token)) {
        Symbol sym;
        sym->type = get_operator_type(token);
        strcpy(sym->symbol[0], token);
        push(stack, sym);

        return true;
    }

    return false;
}

// Retorna o tipo de operador, podendo ser unário ou binário
int get_operator_type(const char token) {
    if (strcasecmp(token, "<sen>") == 0 || strcasecmp(token, "cos") == 0) return SYM_UNARY_OPERATOR;

    return SYM_BINARY_OPERATOR;
}

// Verifica se o token é um comando e insere ele na pilha
bool is_token_command(const char token[8], Stack * stack) {
    if (is_command(token)) {
        Symbol sym;
        sym->type = SYM_COMMAND;
        strcpy(sym->symbol[0], token);
        push(stack, sym);

        return true;
    }

    return false;
}