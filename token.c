#include <stdio.h>   // printf,
#include <string.h>  // strlen, strcpy, strstr, memset, strncpy, strchr
#include <strings.h> // strncasecmp

#include "token.h"

// Varre a string, quebrando-a em tokens, por exemplo, no formato <number>
bool check_tokens(const char * expression) {
    char aux[(int)(strlen(expression)/sizeof(char))];
    char token[(int)(strlen(expression)/sizeof(char))];
    strcpy(aux, expression);

    do {
        if (!get_token(aux, token)){
            printf("Insira tokens válidos!\n");
            return false;
        }

        if (!is_valid_token(token)) {
            printf("O token '%s' é inválido!\n", token);
            return false;
        }
    } while(( (int)(strlen(aux)/sizeof(char)) ) > 0);
    return true;
}

// Busca pelo primeiro token da string, caso seja um token válido, retorna true
bool get_token(char expression[], char token[]) {
    char * startAtPtr = strstr(expression, "<"); // Ponteiro para o primeiro '<'
    char * endAtPtr = strstr(expression, ">");   // Ponteiro para o primeiro '>'
    int startAt = (startAtPtr == NULL ? -1 : startAtPtr - expression); // Índice para o primeiro '<'
    int endAt = (endAtPtr == NULL ? -1 : endAtPtr - expression);       // Índice para o primeiro '>'

    if (startAt != 0) return false;
    if (endAt == -1 || startAt == -1) return false;
    if (startAt > endAt) return false;

    memset(token, 0, strlen(token));     // Limpa o conteúdo do token
    strncpy(token, expression, ++endAt); // Copia o token da expressão para a variável
    token[endAt] = '\0';                 // Força o fim da string

    // Verifica se sobraram caractéres na string (expressão)
    if ((int)(strlen(expression) / sizeof(char) - endAt) == 0) {
        strcpy(expression, "");
    } else {
        // Remove o token da string (expressão)
        char aux[(int)(strlen(expression)/sizeof(char)) - endAt];
        strncpy(aux, expression + endAt, (int)(strlen(expression)/sizeof(char)));
        strcpy(expression, aux);
    }

    return true;
}

// Verifica se o token é válido
bool is_valid_token(const char token[]) {
    if(is_number(token)) return true;
    if(is_operator(token)) return true;
    if(is_command(token)) return true;

    return false;
}

// Verifica se o token é um número ou um dígito que compõe um número
bool is_number(const char token[]) {
    if( ((int)(strlen(token) / sizeof(char))) > 3) return false;

    char digit = (char)token[1];
    return strchr("0123456789Ee.", digit) != NULL;
}

// Verifica se o token é um operador
bool is_operator(const char token[]) {
    if( ((int)(strlen(token) / sizeof(char))) > 5) return false;

    char operators[8][4] = {"+", "-", "*", "/", "^", "log", "sen", "cos"};
    int i;

    for(i = 0; i < 8; i++){
        if(strlen(token) - 2 != strlen(operators[i])) continue;
        if(strncasecmp(token + 1, operators[i], strlen(operators[i])) == 0) return true;
    }

    return false;
}

// Verifica se o token é um comando
bool is_command(const char token[]) {
    if( ((int)(strlen(token) / sizeof(char))) > 7) return false;

    char commands[1][6] = {"enter"};
    int i;

    for(i = 0; i < 1; i++){
        if(strlen(token) - 2 != strlen(commands[i])) continue;
        if(strncasecmp(token + 1, commands[i], strlen(commands[i])) == 0) return true;
    }

    return false;
}
