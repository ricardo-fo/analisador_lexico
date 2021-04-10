#ifndef LEXICAL_ANALYZER_H_INCLUDED
#define LEXICAL_ANALYZER_H_INCLUDED

#include <stdbool.h>

#define SYM_INTEGER 0
#define SYM_FLOAT 1
#define SYM_UNARY_OPERATOR 2
#define SYM_BINARY_OPERATOR 3
#define SYM_COMMAND 4

bool analyse(const char *);

int check_type(char *, char[]);

int is_sym_number(char[], char[]);

int is_sym_operator(char[]);

int is_sym_command(char[]);

bool is_number2(const char[]);

bool get_symbol(char[], char[]);

#endif // LEXICAL_ANALYZER_H_INCLUDED
