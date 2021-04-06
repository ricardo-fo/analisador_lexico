#include "util.h"
#include <stdlib.h> // size_t, realloc
#include <ctype.h>  // isspace, tolower
#include <stdio.h>  // getchar

/*
 * Function: read_string
 * ---------------------
 * Lê uma sequência de caractéres do teclado e cria uma string.
 *
 * returns: um ponteiro para a string gerada.
 */
char * read_string() {
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
