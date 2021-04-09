#ifndef LEXICAL_ANALYZER_H_INCLUDED
#define LEXICAL_ANALYZER_H_INCLUDED

#include "queue.h"

#define MAX_TOKENS 1000
#define MAX_STR_SIZE 8

// Tipos de símbolos aceitos
typedef enum {
    SYM_INTEGER = 0,
    SYM_FLOAT = 1,
    SYM_UNARY_OPERATOR = 2,
    SYM_BINARY_OPERATOR = 3,
    SYM_COMMAND = 4
} SymbolType;

// Tipo de símbolo da sequência que compõe a expressão
typedef struct {
    SymbolType type;
    char symbol[MAX_TOKENS][MAX_STR_SIZE];
} Symbol;

void analyse(Queue *);

#endif // LEXICAL_ANALYZER_H_INCLUDED
