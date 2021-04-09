#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED

#include <stdbool.h> // bool

/* Busca por um token numa string. */
bool get_token(char [], char[]);

/* ... */
// void put_token(char [], char []);

/* Checa se os tokens de uma string são válidos. */
bool check_tokens(const char *);

/* Checa se um token em específico é válido. */
bool is_valid_token(const char []);

/* Checa se um token é um operador matemático. */
bool is_operator(const char []);

/* Checa se um token é um número ou um dígito que compõe um número. */
bool is_number(const char []);

/* Checa se um token é um comando. */
bool is_command(const char []);

#endif // TOKEN_H_INCLUDED
